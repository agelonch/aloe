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

#ifndef HWAPI_PIPELINE_H
#define HWAPI_PIPELINE_H

#include <signal.h>
#include "objects_max.h"
#include "hwapi_process.h"
#include "hwapi_kernel.h"
#include "hwapi_timer.h"

#define PRINT_TIME


/**
 * A ProcThread is a POSIX-thread that runs in a single processor core with
 * real-time priority and FIFO-scheduling (non-preemptable).
 * It has a queue of function pointers which are called one after another.
 * This functions are supposed to run SP tasks.
 * The thread is synchronized with the kernel-thread with a shared memory
 * POSIX semaphore. At the beginning of the timeslot, the kernel-thread
 * actives a semaphore indicating the Proc-Thread may start its execution.
 * After all functions are called, the Proc-Thread blocks on the semaphore again.
 * 
 * These threads are detachable.
 */
typedef struct {
	int id;

	/* pointer to the first process to execute */
	hwapi_process_t *first_process;

	/* The pipeline runs in a thread when synchronizes to another timer */
	pthread_t thread;

	/* The pipeline runs its own timer, which is synchronized at the beginning*/
	hwapi_timer_t mytimer;

	int stop;
	/**
	 * Indicates if the thread has finished the execution of a time-slot or not.
	 */
	int finished;

	int nof_processes;

	int ts_counter;
	/**
	 * Indicates the execution position of the running module
	 */
	hwapi_process_t *running_process;
	int running_process_idx;

}pipeline_t;

void pipeline_run_from_timer(void *arg, struct timespec *time);
void pipeline_sync_threads();
void pipeline_initialize(int _num_pipelines);
void *pipeline_run_thread(void *obj);
int pipeline_recover_thread(pipeline_t *obj);
int pipeline_add(pipeline_t *obj, hwapi_process_t *process);
int pipeline_remove(pipeline_t *obj, hwapi_process_t *proc);

#endif
