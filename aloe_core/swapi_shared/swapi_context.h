#ifndef SWAPI_CONTEXT_H
#define SWAPI_CONTEXT_H

#include "swapi_counter.h"
#include "swapi_itf.h"
#include "swapi_log.h"
#include "swapi_error.h"
#include "objects_max.h"
#include "hwapi.h"

/**
 * This is the module's interface to ALOE. It provides functions for creation of interfaces, global variables, pre-initialized values, log files and time counters. See swapi_module class for information on how to define a module.
 */
typedef struct {
	swapi_itf_t interfaces[MAX(swapi_itf)];
	swapi_counter_t counters[MAX(swapi_counter)];
	swapi_log_t logs[MAX(swapi_log)];

	swapi_error_t error;
	void *module;
	int tstamp;
	h_phyitf_t probeItf;
	int changing_status;
	int closed_resources;
}swapi_context_t;

#endif


