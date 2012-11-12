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


