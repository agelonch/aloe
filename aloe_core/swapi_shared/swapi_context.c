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

/** \brief Calls the function fnc with argument arg
 */
int call_status(int (*fnc)(void*), void *arg, waveform_status_enum new_status) {
	sdebug("fnc=0x%x, context=0x%x, new_status=%d\n",fnc,arg, new_status);
	cast(ctx,arg);
	if (fnc(arg)) {
		return -1;
	}
	nod_module_t *module = (nod_module_t*) ctx->module;
	sdebug("module_id=%d, changing_status=%d, now_is=0\n",module->parent.id,
			module->changing_status);
	module->changing_status = 0;
	module->parent.status = new_status;
	return 0;
}






/************************* PUBLIC FUNCTIONS *********************/



/** \brief Returns a pointer to the swapi name string.
 */
char *swapi_module_name(void *context) {
	cast_p(ctx,context);
	nod_module_t *module = (nod_module_t*) ctx->module;
	sdebug("context=0x%x, module_id=%d, module_name=0x%x\n",context,module->parent.id,
			module->parent.name);

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
	sdebug("context=0x%x, tstamp=%d\n",context,ctx->tstamp);
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

/** \brief Returns a pointer to the first empty log. Before returning, it fills the
 * log name with the second parameter string and sets the id to a positive integer.
 * \returns NULL on error or non-null on success.
 */
log_t swapi_log_create(void *context, string name) {
	sdebug("context=0x%x, name=%s\n",context,name);
	cast_p(ctx,context);
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
	} else {
		ctx->logs[i].id = i+1;
		return (log_t) &ctx->logs[i];
	}
}

/**
 * Use the probeItf to send the variable.reportPacket to the manager
 */
int report_variable(variable_t variable) {
	aerror("Not yet implemented");
	return -1;
}

/**
 * This function is called by the sStop() function and the _abort() functions.
 * It removes the module from the execution pipeline in ProcThread.
 * The next timeslot the module won't be executed.
 */
void swapi_exit(void *context) {
	aerror("Not yet implemented");
}

