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

#ifndef PIPELINE_SYNC_H_
#define PIPELINE_SYNC_H_

#include <pthread.h>
#include <semaphore.h>
#include "futex.h"
#include "objects_max.h"

#define SEM 		1
#define FUTEX 		2
#define BARRIER		3
#define PIPELINESYNC_MUTEX_TYPE	FUTEX

#if PIPELINESYNC_MUTEX_TYPE == SEM
#define pipeline_sync_initialize_ for (int i=0;i<num_pipelines;i++) {\
	sem_init(&semaphores[i], 0, 0); }
#define pipeline_sync_thread_waits(idx) sem_wait(&semaphores[idx])
#define pipeline_sync_threads_wake() for (int i=0;i<num_pipelines;i++) {\
	sem_post(&semaphores[i]); }

static int num_pipelines;
static sem_t semaphores[MAX(processors)];
#endif

#if PIPELINESYNC_MUTEX_TYPE == FUTEX
#define pipeline_sync_initialize_
#define pipeline_sync_thread_waits(idx) futex_wait(&futex)
#define pipeline_sync_threads_wake() futex_wake(&futex)

static int futex;
#endif

#if PIPELINESYNC_MUTEX_TYPE == BARRIER
#define pipeline_sync_initialize_ pthread_barrier_init(&barrier, NULL, \
		(unsigned int) num_pipelines+1)
#define pipeline_sync_thread_waits(idx) pthread_barrier_wait(&barrier)
#define pipeline_sync_threads_wake() pthread_barrier_wait(&barrier)

static pthread_barrier_t barrier;
#endif





#endif
