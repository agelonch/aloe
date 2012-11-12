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
#include "swapi_context.h"
#include "nod_variable.h"
#include "swapi.h"
#include "defs.h"
#include "nod_module.h"

char *swapi_module_name(void *context) {
	swapi_context_t *ctx = (swapi_context_t*) context;
	nod_module_t *my_module = (nod_module_t*) ctx->module;

	return my_module->parent.name;
}

int swapi_module_id(void *context) {
	swapi_context_t *ctx = (swapi_context_t*) context;
	nod_module_t *my_module = (nod_module_t*) ctx->module;

	return my_module->parent.id;
}

/**
 * This function is called by the sStop() function and the _abort() functions. It removes the module from the execution pipeline in ProcThread. The next timeslot the module won't be executed.
 */
void swapi_exit(void *context) {
	aerror("Not yet implemented");
}

/**
 * Returns the module's timestamp. This is the number of timeslots that the object has executed the INIT an RUN states.
 * This function is always successful.
 */
int swapi_tstamp(void *context) {
	aerror("Not yet implemented");
	return -1;
}

/**
 * 1) Find an empty space in +counters
 * 2) call counters[i].create(name)
 * 3) return &counters[i]
 */
counter_t swapi_counter_create(void *context, string name) {
	aerror("Not yet implemented");
	return NULL;
}

/**
 * 1) Find an empty space in +logs
 * 2) call logs[i].create(name)
 * 3) return &logs[i]
 */
log_t swapi_log_create(void *context, string name) {
	aerror("Not yet implemented");
	return NULL;
}

/**
 * Finds an empty space in interfaces, calls itf[i].init() and returns pointer
 */
itf_t swapi_itf_create(void *context, string name, swapi_itf_mode_t mode) {
	aerror("Not yet implemented");
	return NULL;
}

/**
 * Finds an empty space in variables, calls var[i].init() and returns pointer
 */
var_t swapi_var_create(void *context, string name, int size, void **ptr) {
	aerror("Not yet implemented");
	return NULL;
}

/**
 * wrapper to the module's skeleton new state function.
 */
int call_status(int (*fnc)(void)) {
	return -1;
}

/**
 * Use the probeItf to send the variable.reportPacket to the manager
 */
int report_variable(nod_variable_t variable) {
	aerror("Not yet implemented");
	return -1;
}
