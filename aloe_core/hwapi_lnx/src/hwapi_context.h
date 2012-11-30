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

#ifndef HWAPI_CONTEXT_H
#define HWAPI_CONTEXT_H

#include <semaphore.h>

#include "hwapi_itfqueue.h"
#include "hwapi_itfphysic.h"
#include "hwapi_dac.h"
#include "hwapi_machine.h"
#include "hwapi_error.h"
#include "hwapi_periodic.h"
#include "hwapi_time.h"
#include "pipeline.h"
#include "objects_max.h"
#include "hwapi.h"

/**
 * The Base HWAPI class abstracts the platform (OS) and hardware particularities, for instance: timers, shared memory, physical interfaces and process creation/scheduling.
 * 
 * Only one instance of this class can be created on each node. This is created by the kernel-thread class at boot of every node or by the Manager process. The hwapi is in charge of controlling thread-local-storage. Therefore, there is no need to pass a pointer to the hwapi context to each function call. In linux, can be implemented using POSIX TLS or __thread GCC. However, this is not strictly required since only one execution thread access thread-sensible functions at a time.
 */
typedef struct {
	hwapi_error_t error;
	hwapi_machine_t machine;
	hwapi_time_t time;
	hwapi_itfqueue_t queues[MAX(hwapi_itfqueue)];
	hwapi_periodic_t periodic[MAX(hwapi_periodic)];
	int nof_periodic;
	pipeline_t pipelines[MAX(pipeline)];
	hwapi_process_t processes[MAX(hwapi_process)];
	hwapi_dac_t dacs[MAX(hwapi_dac)];
	hwapi_itfphysic_t physic_itfs[MAX(hwapi_itfphysic)];
	sem_t sleep_semaphore;
	int wake_tslot;
} hwapi_context_t;

int hwapi_initialize_node(hwapi_context_t *context, string config_file, void (*ts_begin_fnc)(void));


#endif
