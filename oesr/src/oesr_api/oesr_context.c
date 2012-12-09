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

#include <stddef.h>
#include "rtdal.h"

#include "str.h"
#include "defs.h"
#include "oesr.h"
#include "oesr_context.h"
#include "nod_waveform.h"
#include "waveform.h"

/** \brief Returns the size of the oesr context
 */
size_t oesr_sizeof() {
	return sizeof(oesr_context_t);
}

/** \brief Initializes the oesr context. Sets the context pointer to _module and the
 * module->context pointer to _context
 */
int oesr_context_init(void *_context, void *_module) {
	ndebug("context=0x%x, module=0x%x\n",_context,_module);
	if (!_context) return -1;
	if (!_module) return -1;
	oesr_context_t *context = (oesr_context_t*) _context;
	nod_module_t *module = (nod_module_t*) _module;
	context->module = module;

	context->tstamp = 0;
	context->closed_resources = 0;
	memset(context->counters,0,sizeof(oesr_counter_t)*MAX(oesr_counter));
	memset(context->logs,0,sizeof(oesr_log_t)*MAX(oesr_log));
	module->changing_status = 0;
	return 0;
}

nod_module_t *oesr_get_module(void *context) {
	if (!context) return NULL;
	oesr_context_t *ctx = (oesr_context_t*) context;
	return (nod_module_t*) ctx->module;
}





/************************* PUBLIC FUNCTIONS *********************/



/** \brief Returns a pointer to the oesr name string.
 */
char *oesr_module_name(void *context) {
	cast_p(ctx,context);
	nod_module_t *module = (nod_module_t*) ctx->module;
	return module->parent.name;
}

/** \brief Returns the module id (non-negative integer), -1 on error
 */
int oesr_module_id(void *context) {
	cast(ctx,context);
	nod_module_t *module = (nod_module_t*) ctx->module;
	sdebug("context=0x%x, module_id=%d, module_name=0x%x\n",context,module->parent.id,
			module->parent.name);

	return module->parent.id;
}


/**
 * \brief Returns the module's timestamp. This is the number of timeslots that the object has executed
 * the INIT an RUN states. This function is always successful.
 */
int oesr_tstamp(void *context) {
	cast(ctx,context);
	return ctx->tstamp;
}

int oesr_tslot_length(void *context) {
	rtdal_machine_t machine;
	rtdal_machine(&machine);
	return machine.ts_len_us;
}

/** \brief A counter shall be used by a user to measure time intervals. oesr_counter_create() returns
 * a counter_t object which is passed as a first parameter to oesr_counter_start() and
 * oesr_counter_stop() functions, which start and stop the counter, respectively. A counter is
 * associated with a public module variable which can be accessed from the oesr_man interface. The
 * variable value is updated during a call to oesr_counter_stop(), saving the elapsed time between
 * successive calls to start() and stop(), in microseconds.
 *
 * oesr_counter_create() returns a pointer to the first empty counter. Before returning, it fills the
 * counter name with the second parameter string and sets the id to a positive integer.
 *
 * \param context Pointer to the oesr context
 * \param name Name associated to the public variable
 * \returns On success, returns a non-null counter_t object. On error returns null.
 */
counter_t oesr_counter_create(void *context, string name) {
	sdebug("context=0x%x, name=%s\n",context,name);
	cast_p(ctx,context);
	oesr_ASSERT_PARAM_P(name);
	int i;

	i=0;
	while(i<MAX(oesr_counter) && ctx->counters[i].id)
		i++;
	if (i==MAX(oesr_counter)) {
		oesr_SETERROR(oesr_ERROR_NOSPACE);
		return NULL;
	}
	sdebug("counter_pos=%d\n",i);
	if (oesr_counter_init(ctx, &ctx->counters[i],name)) {
		return NULL;
	} else {
		ctx->counters[i].id = i+1;
		return (counter_t) &ctx->counters[i];
	}
}

/** \brief Returns a log handler used by the module to read/write logs. Find an empty log in
 * the logs array and initializes it usign oesr_log_init(). The returned handler (if non-null)
 * is then passed as the first parameter to the functions oesr_log_write() or oesr_log_printf().
 * \returns NULL on error or non-null on success.
 */
log_t oesr_log_create(void *context, string name) {
	sdebug("context=0x%x, name=%s\n",context,name);
	cast_p(ctx,context);
	oesr_ASSERT_PARAM_P(name);
	int i;

	i=0;
	while(i<MAX(oesr_log) && ctx->logs[i].id)
		i++;
	if (i==MAX(oesr_log)) {
		oesr_SETERROR(oesr_ERROR_NOSPACE);
		return NULL;
	}
	if (oesr_log_init(ctx, &ctx->logs[i], name)) {
		return NULL;
	}
	ctx->logs[i].id = i+1;
	return (log_t) &ctx->logs[i];

}

/**
 * Use the probeItf to send the variable.reportPacket to the manager
 */
int report_variable(variable_t variable) {
	aerror("Not yet implemented");
	return -1;
}

/** \brief Stops the calling module execution. Sets the module as NON-runnable.
 *
 * The module continues the execution after this function returns but, once the module returns from one
 * of the Init/Run/Stop functions, it will not be executed again.
 * The oesr_exit() function DOES NOT free the module's resources. In the case of a normal STOP,
 * this is done by the same function that allocated them, that is, the nod_waveform_unserializeTo().
 */
int oesr_exit(void *context) {
	cast(ctx,context);
	nod_module_t *module = (nod_module_t*) ctx->module;
	sdebug("module_id=%d, cur_status=%d\n",module->parent.id, module->parent.status);
	if (!module->process) {
		return -1;
	}
	if (rtdal_process_stop(module->process)) {
		return -1;
	}
	return 0;
}

