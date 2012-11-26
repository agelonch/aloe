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

static int nod_module_load(nod_module_t *module);
static int nod_module_run(nod_module_t *module);
static int nod_module_remove(nod_module_t *module);
static int nod_module_status_ok(nod_module_t *module, waveform_status_enum w_status);




/**************** WAVEFORM Functions *****************/




/** \brief Allocates resources for nof_modules in a nod_waveform_t waveform.
 * Does NOT call allocate interfaces/variables for each module.
 */
int nod_waveform_alloc(nod_waveform_t *w, int nof_modules) {
	ndebug("waveform_id=%d, nof_modules=%d\n",w->id, nof_modules);
	aassert(w);
	w->modules = (nod_module_t*) pool_alloc(nof_modules,sizeof(nod_module_t));
	if (!w->modules) return -1;
	w->nof_modules = nof_modules;
	return 0;
}

/** \brief Deallocates nod_waveform_t resources allocated using nod_waveform_alloc().
 * Does NOT call deallocate interfaces/variables for each module.
 */
int nod_waveform_free(nod_waveform_t *w) {
	ndebug("waveform_id=%d, nof_modules=%d\n",w->id, w->nof_modules);
	aassert(w);
	if (pool_free(w->modules)) return -1;
	w->modules = NULL;
	w->nof_modules = 0;
	return 0;
}

/* \brief nod_waveform_load() calls nod_module_load() for each module in the waveform
 */
int nod_waveform_load(nod_waveform_t *w) {
	ndebug("waveform_id=%d, nof_modules=%d\n",w->id, w->nof_modules);
	aassert(w);
	int i;
	for (i=0;i<w->nof_modules;i++) {
		if (nod_module_load(&w->modules[i])) {
			return -1;
		}
	}
	return 0;
}

/* \brief nod_waveform_run() calls nod_module_run() for each module in the waveform
 */
int nod_waveform_run(nod_waveform_t *w) {
	ndebug("waveform_id=%d, nof_modules=%d\n",w->id, w->nof_modules);
	aassert(w);
	int i;
	for (i=0;i<w->nof_modules;i++) {
		if (nod_module_run(&w->modules[i])) {
			return -1;
		}
	}
	w->status.cur_status = LOADED;
	return 0;
}

/* \brief nod_waveform_remove() calls nod_module_remove() for each module in the waveform and
 * sets status = STOP, id = 0 and CALLS nod_waveform_free().
 */
int nod_waveform_remove(nod_waveform_t *w) {
	ndebug("waveform_id=%d, nof_modules=%d, status=%d\n",w->id, w->nof_modules,
			w->status.cur_status);
	aassert(w);
	int i;
	for (i=0;i<w->nof_modules;i++) {
		if (nod_module_remove(&w->modules[i])) {
			return -1;
		}
	}
	if (nod_waveform_free(w)) {
		return -1;
	}
	w->status.cur_status = STOP;
	w->id = 0;
	return 0;
}

/** \brief Verifies that the status has been changed correctly. Sets the caller thread to a
 * waiting state until w->status.dead_timeslot. When it wakes, it calls nod_module_status_ok()
 * for each module.
 * \returns 0 if the status changed correctly, or -1 if any module did not change the status
 */
int nod_waveform_status_ok(nod_waveform_t *w) {
	ndebug("waveform_id=%d, nof_modules=%d, status=%d\n",w->id, w->nof_modules,
				w->status.cur_status);
	int i;
	aassert(w);

	if (hwapi_sleep(w->status.dead_timeslot)) {
		return -1;
	}

	for (i=0;i<w->nof_modules;i++) {
		if (nod_module_status_ok(&w->modules[i], w->status.cur_status)) {
			return -1;
		}
	}

	return 0;
}

/** \brief Serializes a nod_waveform_t object to a packet.
 * \param all_module If non-zero, serializes the entire module structure. If is zero serializes
 * only the execinfo structure
 * \param copy_data Indicates what kind of data will be send for each variable. See enum variable_serialize_data
 */
int nod_waveform_serialize(nod_waveform_t *src, packet_t *pkt, int all_module,
		enum variable_serialize_data copy_data) {
	ndebug("waveform_id=%d, nof_modules=%d, status=%d, all_module=%d, copy_data=%d\n",
			src->id, src->nof_modules,src->status.cur_status, all_module, copy_data);
	int i;
	aassert(pkt);
	aassert(src);

	add_i(&all_module);
	add_i(&src->nof_modules);
	ndebug("nof_modules=%d\n",src->nof_modules);
	for (i=0;i<src->nof_modules;i++) {
		add_i(&src->modules[i].parent.id);
		if (all_module) {
			if (module_serialize(&src->modules[i].parent,pkt,copy_data))
				return -1;
		} else {
			if (execinfo_serialize(&src->modules[i].parent.execinfo,pkt))
				return -1;
		}
	}
	return 0;
}

/** \brief Unserializes a nod_waveform_t object from the packet to the structure pointed by dest.
 * If the waveform is in status STOP unserializes all the waveform contents. It unserializes
 * the waveform status only otherwise.
 * The copy_data parameter is obtained from the packet.
 * nod_waveform_unserializeTo() calls nod_waveform_alloc() to allocate memory for the modules
 * and nod_module_alloc() to allocate the swapi context memory for each module.
 */
int nod_waveform_unserializeTo(packet_t *pkt, nod_waveform_t *dest) {
	ndebug("waveform_id=%d, nof_modules=%d, status=%d,\n",
			dest->id, dest->nof_modules,dest->status.cur_status);
	int i;
	int nof_modules, tmp;
	enum variable_serialize_data copy_data;
	aassert(pkt);
	aassert(dest);

	if (dest->status.cur_status == STOP) {
		get_i(&tmp);
		copy_data = (enum variable_serialize_data) tmp;
		get_i(&dest->id);
		if (packet_get_data(pkt,dest->name,STR_LEN)) return -1;

		get_i(&dest->nof_modes);
		for (i=0;i<dest->nof_modes;i++) {
			if (waveform_mode_unserializeTo(pkt,&dest->modes[i])) {
				return -1;
			}
		}

		get_i(&nof_modules);
		ndebug("id=%d, copy_data=%d, nof_modules=%d\n",dest->id,tmp,nof_modules);
		if (nod_waveform_alloc(dest,nof_modules)) return -1;
		for (i=0;i<dest->nof_modules;i++) {
			if (nod_module_alloc(&dest->modules[i])) return -1;
			dest->modules[i].parent.waveform = dest;
			if (module_unserializeTo(pkt,&dest->modules[i].parent, copy_data))
				return -1;
		}
	} else {
		if (packet_get_data(pkt, &dest->status, sizeof(waveform_status_t))) {
			return -1;
		}
		if (nod_waveform_status_ok(dest)) {
			return -1;
		}
	}
	return 0;
}







/**************** MODULE Functions *****************/



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
	ndebug("module_id=%d, addr=0x%x, context=0x%x\n",module->parent.id,module,module->context);
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

/** \brief nod_module_load() initializes the module's swapi context and then uses the hwapi
 * to create a new process that will run the module functions. The swapi context pointer is passed
 * as a parameter to the hwapi_process_new(). This pointer will be passed to the module entry point
 * functions.
 * \returns 0 on success, -1 on error
 */
static int nod_module_load(nod_module_t *module) {
	ndebug("module_id=%d, binary=%s, exe_pos=%d, pidx=%d, context=0x%x\n",module->parent.id,
			module->parent.binary, module->parent.exec_position,module->parent.processor_idx,
			module->context);
	aassert(module);
	struct hwapi_process_attr attr;

	if (swapi_context_init(module->context, module)) {
		return -1;
	}

	memset(&attr, 0, sizeof(struct hwapi_process_attr));
	strcpy(attr.binary_path,module->parent.binary);
	attr.exec_position = module->parent.exec_position;
	attr.pipeline_id = module->parent.processor_idx;

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
static int nod_module_run(nod_module_t *module) {
	ndebug("module_id=%d\n",module->parent.id);
	aassert(module);
	if (hwapi_process_run(module->process)) {
		hwapi_perror("hwapi_process_run");
		return -1;
	}
	return 0;
}

/** \brief Removes the module's from the hwapi pipeline. Calls module_free() to dealloc
 * the interfaces/variables memory and then nod_module_free() to dealloc the swapi context memory.
 * Sets the id to zero before finishing.
 */
static int nod_module_remove(nod_module_t *module) {
	ndebug("module_id=%d\n",module->parent.id);
	aassert(module);
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
static int nod_module_status_ok(nod_module_t *module, waveform_status_enum w_status) {
	ndebug("module_id=%d, changing_status=%d, module_status=%d, waveform_status=%d\n",
			module->parent.id, module->changing_status, module->parent.status, w_status);
	aassert(module);

	if (module->changing_status)
		return -1;
	if (module->parent.status != w_status) {
		return -1;
	}
	return 0;
}

/** \brief Returns a pointer to the first module's variable with name equal to the second parameter
 *
 */
variable_t* nod_module_variable_get(nod_module_t *module, string name) {
	ndebug("module_id=%d, nof_variables=%d, name=%s\n",module->parent.id,
			module->parent.nof_variables, name);
	int i;
	if (!module || !name) {
		return NULL;
	}
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






/**************** VARIABLE Functions *****************/



/** \brief Allocates memory for the variable value. Since all memory is shared between threads,
 * the memory is allocated using the common mempool. Allocates size bytes.
 * This function is called from the swapi when a new module creates a new variables.
 */
int nod_variable_init(variable_t *variable, int size) {
	ndebug("variable_id=%d, size=%d, current_size=%d\n",variable->id,variable->size,size);
	if (!variable) {
		return -1;
	}
	if (size<0) {
		return -1;
	}
	if (variable->cur_value) {
		return -1;
	}
	variable->cur_value = pool_alloc(size, 1);
	if (!variable->cur_value) {
		return -1;
	}
	variable->size = size;
	return 0;
}

/** \brief Deallocates the memory allocated by nod_variable_init()
 */
int nod_variable_close(variable_t *variable) {
	ndebug("variable_id=%d, size=%d\n",variable->id,variable->size);
	if (!variable) {
		return -1;
	}
	if (variable->cur_value == NULL) {
		return -1;
	}
	if (pool_free(variable->cur_value)) {
		return -1;
	}
	variable->id = 0;
	variable->size = 0;
	return 0;
}




