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

#ifndef rtdal_CONTEXT_H
#define rtdal_CONTEXT_H

#include <pthread.h>

#include "rtdal_itfspscq.h"
#include "rtdal_itfphysic.h"
#include "rtdal_dac.h"
#include "rtdal_machine.h"
#include "rtdal_error.h"
#include "rtdal_periodic.h"
#include "rtdal_time.h"
#include "pipeline.h"
#include "objects_max.h"
#include "rtdal.h"

/**
 * The Base rtdal class abstracts the platform (OS) and hardware particularities, for instance: timers, shared memory, physical interfaces and process creation/scheduling.
 * 
 * Only one instance of this class can be created on each node. This is created by the kernel-thread class at boot of every node or by the Manager process. The rtdal is in charge of controlling thread-local-storage. Therefore, there is no need to pass a pointer to the rtdal context to each function call. In linux, can be implemented using POSIX TLS or __thread GCC. However, this is not strictly required since only one execution thread access thread-sensible functions at a time.
 */
typedef struct {
	rtdal_error_t error;
	rtdal_machine_t machine;
	rtdal_time_t time;
	rtdal_itfspscq_t spscqs[MAX(rtdal_itfspscq)];
	rtdal_periodic_t periodic[MAX(rtdal_periodic)];
	int nof_periodic;
	pipeline_t pipelines[MAX(pipeline)];
	rtdal_process_t processes[MAX(rtdal_process)];
	rtdal_dac_t dacs[MAX(rtdal_dac)];
	rtdal_itfphysic_t physic_itfs[MAX(rtdal_itfphysic)];
	int wake_tslot;
	pthread_mutex_t mutex;
} rtdal_context_t;

int rtdal_initialize_node(rtdal_context_t *context, string config_file, void (*ts_begin_fnc)(void));


#endif
