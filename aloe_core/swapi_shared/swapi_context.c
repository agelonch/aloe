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
