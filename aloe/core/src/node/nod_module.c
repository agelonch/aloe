/*
 * Copyright (c) 2012, Ismael Gomez-Miguelez <ismael.gomez@tsc.upc.edu>.
 * This file is part of ALOE++ (http://flexnets.upc.edu/)
 *
 * ALOE++ is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * ALOE++ is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with ALOE++.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "defs.h"
#include "packet.h"
#include "hwapi.h"
#include "nod_waveform.h"
#include "mempool.h"
#include "swapi_context.h"


/** \brief Allocates memory for the SWAPI context structure of a nod_module_t
 *
 */
int nod_module_alloc(nod_module_t *module) {
	ndebug("module_addr=0x%x\n",module);
	aassert(module);

	module->context = pool_alloc(1,swapi_sizeof());
	if (!module->context) {
		return -1;
	}

	if (swapi_context_init(module->context, module)) {
		return -1;
	}

	return 0;
}

/** \brief Deallocates the SWAPI structure memory allocated using nod_module_alloc()
 *
 */
int nod_module_free(nod_module_t *module) {
	ndebug("module_id=%d, addr=0x%x, context=0x%x\n",module->parent.id,module,module->context);
	aassert(module);

	module->context = pool_alloc(1,swapi_sizeof());
	if (!module->context) {
		return -1;
	}
	module->context = NULL;
	return 0;
}


/**
 * \brief This function is called by the HWAPI when a module finishes the execution. It is always
 * called from a non-priority task, therefore there are no time constraints.
 *
 * If process.finishCode!=ProcessErrorCode.OK, an excepcional error occurred.
 * The reason is stored in process.errorMsg string.
 */
void *nod_module_finish_callback(void *context) {
	nod_module_t *module = swapi_get_module(context);
	nod_waveform_t *waveform = module->parent.waveform;
	ndebug("module_id=%d, error_code=%d, finishing=%d\n",module->parent.id,
			hwapi_process_geterror(module->process), waveform->finishing);

	switch(hwapi_process_geterror(module->process)) {
	case FINISH_OK:
		aerror("Warning, not supposed to be here\n");
		return 0;
	case RTFAULT:
	case SIG_RECV:
	case RUNERROR:
		aerror_msg("Module %s terminated abnormally. Trying a clean stop\n",
						module->parent.name);

		/**@FIXME: Should use a MUTEX here */
		if (waveform->status.cur_status == STOP) {
			ndebug("waveform stopping, module_id=%d\n",module->parent.id);
			if (hwapi_process_run(module->process)) {
				aerror("run\n");
			}
		} else {
			if (nod_waveform_stop(waveform)) {
				aerror("stopping waveform\n");
			}
		}
		return NULL;
	}
	return NULL;
}

/** \brief nod_module_load() initializes the module's swapi context and then uses the hwapi
 * to create a new process that will run the module functions. The swapi context pointer is passed
 * as a parameter to the hwapi_process_new(). This pointer will be passed to the module entry point
 * functions.
 * \returns 0 on success, -1 on error
 */
int nod_module_load(nod_module_t *module) {
	ndebug("module_id=%d, binary=%s, exe_pos=%d, pidx=%d, context=0x%x\n",module->parent.id,
			module->parent.binary, module->parent.exec_position,module->parent.processor_idx,
			module->context);
	aassert(module);
	struct hwapi_process_attr attr;

	memset(&attr, 0, sizeof(struct hwapi_process_attr));
	strcpy(attr.binary_path,module->parent.binary);
	attr.exec_position = module->parent.exec_position;
	attr.pipeline_id = module->parent.processor_idx;
	attr.finish_callback = nod_module_finish_callback;

	module->process = hwapi_process_new(&attr, module->context);
	if (module->process == NULL) {
		hwapi_perror("hwapi_process_new");
		return -1;
	}
	return 0;
}

/** \brief Sets the module's process as runnable
 *
 */
int nod_module_run(nod_module_t *module) {
	ndebug("module_id=%d\n",module->parent.id);
	aassert(module);
	if (hwapi_process_run(module->process)) {
		hwapi_perror("hwapi_process_run");
		return -1;
	}
	return 0;
}

void nod_module_kill_status_task(nod_module_t *module) {
	ndebug("module_id=%d killing=%d\n",module->parent.id,module->status_task);
	if (hwapi_task_kill(module->status_task)) {
		aerror("hwapi_task_kill");
	}
	ndebug("module_id=%d waiting=%d\n",module->parent.id,module->status_task);
	if (hwapi_task_wait(module->status_task,NULL)) {
		module->status_task = 0;
	}
	ndebug("module_id=%d status_task terminated\n",module->parent.id);
}

/** \brief Removes the module's from the hwapi pipeline. Calls module_free() to dealloc
 * the interfaces/variables memory and then nod_module_free() to dealloc the swapi context memory.
 * Sets the id to zero before finishing.
 */
int nod_module_remove(nod_module_t *module) {
	ndebug("module_id=%d\n",module->parent.id);
	aassert(module);

	/* before killing process, make sure there is any thread still running */
	if (module->status_task) {
		nod_module_kill_status_task(module);
	}

	if (module->process) {
		if (hwapi_process_remove(module->process)) {
			hwapi_perror("hwapi_process_remove");
			return -1;
		}
	}
	if (module_free(&module->parent)) {
		return -1;
	}
	if (nod_module_free(module)) {
		return -1;
	}
	module->changing_status = 0;
	module->parent.id = 0;
	return 0;
}

/** \brief Returns 0 if the module changed its status correctly, -1 otherwise.
 *
 */
int nod_module_status_ok(nod_module_t *module, waveform_status_enum w_status) {
	ndebug("module_id=%d, changing_status=%d, module_status=%d, waveform_status=%d\n",
			module->parent.id, module->changing_status, module->parent.status, w_status);
	aassert(module);

	if (module->changing_status) {
		return -1;
	}

	if (module->parent.status != w_status) {
		return -1;
	}
	if (module->status_task) {
		ndebug("status_task=0x%x, waiting\n",module->status_task);
		if (hwapi_task_wait(module->status_task,NULL)) {
			module->status_task = 0;
		}
		module->status_task = 0;
	}
	return 0;
}

/** \brief Returns a pointer to the first module's variable with name equal to the second parameter
 *
 */
variable_t* nod_module_variable_get(nod_module_t *module, string name) {
	ndebug("module_id=%d, nof_variables=%d, name=%s\n",module->parent.id,
			module->parent.nof_variables, name);
	aassert_p(module);
	aassert_p(name);
	int i;
	i=0;
	while(i < module->parent.nof_variables && strcmp(name,module->parent.variables[i].name))
		i++;
	if (i == module->parent.nof_variables) {
		return NULL;
	}
	return &module->parent.variables[i];
}

/** \brief Returns a pointer to the first empty variable in the module structure. Fills
 * the variable name with the second parameter string and sets the variable id to a non-zero integer.
 */
variable_t* nod_module_variable_create(nod_module_t *module, string name) {
	ndebug("module_id=%d, nof_variables=%d, name=%s\n",module->parent.id,
				module->parent.nof_variables, name);
	int i;
	if (!module || !name) {
		return NULL;
	}
	i=0;
	while(i < module->parent.nof_variables && !module->parent.variables[i].id)
		i++;
	if (i == module->parent.nof_variables) {
		ndebug("calling realloc for %d more variables\n",5);
		/**@FIXME: Preallocated more variables */
		module->parent.variables=pool_realloc(module->parent.variables,
				module->parent.nof_variables+5, sizeof(variable_t));
		if (!module->parent.variables) return NULL;
		module->parent.nof_variables+=5;
		ndebug("nof_variables=%d\n",module->parent.nof_variables);
	}

	module->parent.variables[i].id = i+1;
	strncat(module->parent.variables[i].name,name,STR_LEN);

	return &module->parent.variables[i];
}


int nod_module_execinfo_add_sample(execinfo_t *obj, int cpu, int relinquish) {
	aerror("Not yet implemented");
	return -1;
}


