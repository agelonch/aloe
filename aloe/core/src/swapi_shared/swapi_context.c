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
#include "hwapi.h"

#include "str.h"
#include "defs.h"
#include "swapi.h"
#include "swapi_context.h"
#include "nod_waveform.h"
#include "waveform.h"

/** \brief Returns the size of the swapi context
 */
size_t swapi_sizeof() {
	return sizeof(swapi_context_t);
}

/** \brief Initializes the swapi context. Sets the context pointer to _module and the
 * module->context pointer to _context
 */
int swapi_context_init(void *_context, void *_module) {
	ndebug("context=0x%x, module=0x%x\n",_context,_module);
	if (!_context) return -1;
	if (!_module) return -1;
	swapi_context_t *context = (swapi_context_t*) _context;
	nod_module_t *module = (nod_module_t*) _module;
	context->module = module;

	context->tstamp = 0;
	context->closed_resources = 0;
	memset(context->counters,0,sizeof(swapi_counter_t)*MAX(swapi_counter));
	memset(context->logs,0,sizeof(swapi_log_t)*MAX(swapi_log));
	module->changing_status = 0;
	return 0;
}

nod_module_t *swapi_get_module(void *context) {
	if (!context) return NULL;
	swapi_context_t *ctx = (swapi_context_t*) context;
	return (nod_module_t*) ctx->module;
}





/************************* PUBLIC FUNCTIONS *********************/



/** \brief Returns a pointer to the swapi name string.
 */
char *swapi_module_name(void *context) {
	cast_p(ctx,context);
	nod_module_t *module = (nod_module_t*) ctx->module;
	return module->parent.name;
}

/** \brief Returns the module id (non-negative integer), -1 on error
 */
int swapi_module_id(void *context) {
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
int swapi_tstamp(void *context) {
	cast(ctx,context);
	return ctx->tstamp;
}

/** \brief A counter shall be used by a user to measure time intervals. swapi_counter_create() returns
 * a counter_t object which is passed as a first parameter to swapi_counter_start() and
 * swapi_counter_stop() functions, which start and stop the counter, respectively. A counter is
 * associated with a public module variable which can be accessed from the MANAPI interface. The
 * variable value is updated during a call to swapi_counter_stop(), saving the elapsed time between
 * successive calls to start() and stop(), in microseconds.
 *
 * swapi_counter_create() returns a pointer to the first empty counter. Before returning, it fills the
 * counter name with the second parameter string and sets the id to a positive integer.
 *
 * \param context Pointer to the swapi context
 * \param name Name associated to the public variable
 * \returns On success, returns a non-null counter_t object. On error returns null.
 */
counter_t swapi_counter_create(void *context, string name) {
	sdebug("context=0x%x, name=%s\n",context,name);
	cast_p(ctx,context);
	SWAPI_ASSERT_PARAM_P(name);
	int i;

	i=0;
	while(i<MAX(swapi_counter) && ctx->counters[i].id)
		i++;
	if (i==MAX(swapi_counter)) {
		SWAPI_SETERROR(SWAPI_ERROR_NOSPACE);
		return NULL;
	}
	sdebug("counter_pos=%d\n",i);
	if (swapi_counter_init(ctx, &ctx->counters[i],name)) {
		return NULL;
	} else {
		ctx->counters[i].id = i+1;
		return (counter_t) &ctx->counters[i];
	}
}

/** \brief Returns a log handler used by the module to read/write logs. Find an empty log in
 * the logs array and initializes it usign swapi_log_init(). The returned handler (if non-null)
 * is then passed as the first parameter to the functions swapi_log_write() or swapi_log_printf().
 * \returns NULL on error or non-null on success.
 */
log_t swapi_log_create(void *context, string name) {
	sdebug("context=0x%x, name=%s\n",context,name);
	cast_p(ctx,context);
	SWAPI_ASSERT_PARAM_P(name);
	int i;

	i=0;
	while(i<MAX(swapi_log) && ctx->logs[i].id)
		i++;
	if (i==MAX(swapi_log)) {
		SWAPI_SETERROR(SWAPI_ERROR_NOSPACE);
		return NULL;
	}
	sdebug("log_pos=%d\n",i);
	if (swapi_log_init(ctx, &ctx->logs[i], name)) {
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
 * The swapi_exit() function DOES NOT free the module's resources. In the case of a normal STOP,
 * this is done by the same function that allocated them, that is, the nod_waveform_unserializeTo().
 */
int swapi_exit(void *context) {
	cast(ctx,context);
	nod_module_t *module = (nod_module_t*) ctx->module;
	sdebug("module_id=%d, cur_status=%d\n",module->parent.id, module->parent.status);
	if (!module->process) {
		return -1;
	}
	if (hwapi_process_stop(module->process)) {
		return -1;
	}
	return 0;
}

