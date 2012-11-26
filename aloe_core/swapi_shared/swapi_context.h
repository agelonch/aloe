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
#include "swapi_log.h"
#include "swapi_error.h"
#include "objects_max.h"
#include "hwapi.h"
#include "waveform.h"

typedef struct {
	swapi_counter_t counters[MAX(swapi_counter)];
	swapi_log_t logs[MAX(swapi_log)];

	swapi_error_t error;
	void *module;
	int tstamp;
	h_itf_t probeItf;
	int closed_resources;
}swapi_context_t;

int call_status(int (*fnc)(void*), void *arg, waveform_status_enum new_status);
size_t swapi_sizeof();
int swapi_context_init(void *context, void *module);

#define cast(a,b) swapi_context_t *a = (swapi_context_t*) b; SWAPI_ASSERT_PARAM(a)
#define cast_p(a,b) swapi_context_t *a = (swapi_context_t*) b; SWAPI_ASSERT_PARAM_P(a)

#endif


