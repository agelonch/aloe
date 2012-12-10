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

#ifndef oesr_CONTEXT_H
#define oesr_CONTEXT_H

#include "oesr_counter.h"
#include "oesr_log.h"
#include "oesr_error.h"
#include "objects_max.h"
#include "rtdal.h"
#include "waveform.h"
#include "nod_waveform.h"

typedef struct {
	oesr_counter_t counters[MAX(oesr_counter)];
	oesr_log_t logs[MAX(oesr_log)];

	oesr_error_t error;
	void *module;
	int tstamp;
	r_itf_t probeItf;
	int closed_resources;
}oesr_context_t;

size_t oesr_sizeof();
int oesr_context_init(void *context, void *module);
nod_module_t *oesr_get_module(void *context);

#define cast(a,b) oesr_context_t *a = (oesr_context_t*) b; OESR_ASSERT_PARAM(a)
#define cast_p(a,b) oesr_context_t *a = (oesr_context_t*) b; OESR_ASSERT_PARAM_P(a)

#endif


