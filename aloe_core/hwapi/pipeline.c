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

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <pthread.h>
#include <ucontext.h>
#include <execinfo.h>
#include <sys/ucontext.h>

#include "pipeline.h"
#include "hwapi.h"
#include "hwapi_error.h"
#include "hwapi_time.h"
#include "hwapi_kernel.h"
#include "pipeline_sync.h"

static int timer_first_cycle = 0;
static int num_pipelines;
static int is_first_in_cycle_count;


void pipeline_initialize(int _num_pipelines) {
	num_pipelines = _num_pipelines;
	pipeline_sync_initialize_;
}

inline void pipeline_sync_threads() {
	pipeline_sync_threads_wake();
}

inline static void pipeline_run_thread_run_module(pipeline_t *pipe,
		hwapi_process_t *proc, int idx) {

	if (proc->runnable) {
		pipe->running_process = proc;
		pipe->running_process_idx = idx;
		if (proc->run_point(proc->arg)) {
			aerror_msg("Error running module %d:%d\n",
					pipe->id,pipe->running_process_idx);
			pipeline_remove(pipe,proc);
		}
	}
}

inline static void pipeline_run_thread_print_time(pipeline_t *obj) {
#ifdef PRINT_TIME
	time_t tdata;
	hwapi_time_get(&tdata);
	if (!(obj->ts_counter%10000))
		printf("Pipeline %d running %d modules at TS=%d\t Start: %d:%d\n",obj->id, obj->nof_processes,
				hwapi_time_slot(),
				(int) tdata.tv_sec, (int) tdata.tv_usec);
#endif

}
/**
 * Atomic modulo num_pipelines counter.
 */
inline static int is_first_in_cycle() {
	return __sync_fetch_and_add(&is_first_in_cycle_count, 1)
			& (num_pipelines - 1);
}

inline static void pipeline_run_time_slot(pipeline_t *obj) {
	int idx;
	hwapi_process_t *run_proc;

	obj->finished = 0;

	pipeline_run_thread_print_time(obj);

	run_proc = obj->first_process;
	idx = 0;

	while(run_proc) {
		if (idx > obj->nof_processes) {
			aerror_msg("Fatal error. Corrupted pipeline-%d process list at process %d\n",
					obj->id, idx);
			kill(getpid(),SIGTERM);
			pthread_exit(NULL);
		}
		pipeline_run_thread_run_module(obj,run_proc, idx);
		run_proc = run_proc->next;
		idx++;
	}
	obj->ts_counter++;
	obj->finished = 1;
}



void pipeline_run_from_timer(void *arg, struct timespec *time) {
	pipeline_t *obj = (pipeline_t*) arg;

	if (!timer_first_cycle && time) {
		hwapi_time_reset_realtime(time);
		timer_first_cycle = 1;
	}

	if (!is_first_in_cycle()) {
		hwapi_kernel_tasks();
	}

	pipeline_run_time_slot(obj);
}

/**
 * Runs one process after another (calling process[i].run_point()) and then
 * sleeps waiting for the semaphore
 */
void *pipeline_run_thread(void *self) {
	assert(self);
	pipeline_t *obj = (pipeline_t*) self;
	assert(obj->id>=0);

	obj->stop = 0;
	pipeline_sync_thread_waits(obj->id);

	while(!obj->stop) {
		pipeline_run_time_slot(obj);
		pipeline_sync_thread_waits(obj->id);
	}
	printf("%d go out %d\n",obj->id, obj->stop);
	return NULL;
}

/**
 * This function is called by the kernel or signal threads (despite it is in ProcThread class). It is used to restore the thread execution after a fatal aerror occurred in a module (e.g. real-time failure or sigsegv). It prevents the running process from executing, creates a new thread associated to the ProcThread object and kills the thread that caused failure.
 */
int pipeline_recover_thread(pipeline_t *obj) {
	aerror("Not yet implemented");
	return -1;
}
/**
 * Adds a process to the pipeline. It is inserted in the position
 * min(n,exec_position) where n is the number of
 * elements in the queue and exec_position is defined in the process attributes
 * used in the call to hwapi_process_new().
 * Returns the position it has finally been inserted.
 *
 *  @param obj Pointer to the pipeline_t object where the process is inserted
 *  @pram process pointer to the hwapi_process_t object to insert.
 *  @returns non-negative integer number indicating the position it has been
 *  inserted, or -1 on error.
 */
int pipeline_add(pipeline_t *obj, hwapi_process_t *process) {
	HWAPI_ASSERT_PARAM(obj);
	HWAPI_ASSERT_PARAM(process);

	int i, obj_pos;
	hwapi_process_t *cur, *prev;

	obj_pos = process->attributes.exec_position;

	i = 0;
	cur = obj->first_process;
	if (!cur) {
		obj->first_process = process;
		process->next = NULL;
	} else {
		while(cur && i<obj_pos) {
			prev = cur;
			cur = cur->next;
			i++;
		}
		if (cur) {
			process->next = cur->next;
		} else {
			process->next = NULL;
		}
		/* should be not null */
		assert(prev);
		prev->next = process;

	}

	obj->nof_processes++;
	/* assign pipeline to object */
	process->pipeline = obj;

	return i;
}

/**
 * Removes the process proc from the pipeline pointed by obj.
 * @param obj Pointer to the pipeline object
 * @param proc Pointer to the process to remove
 * @return Zero on success, -1 on error
 */
int pipeline_remove(pipeline_t *obj, hwapi_process_t *proc) {
	HWAPI_ASSERT_PARAM(obj);
	HWAPI_ASSERT_PARAM(proc);

	hwapi_process_t *cur, *prev;

	prev = NULL;
	cur = obj->first_process;
	while(cur != proc && cur) {
		prev = cur;
		cur = cur->next;
	}
	if (!cur) {
		HWAPI_SETERROR(HWAPI_ERROR_NOTFOUND);
		return -1;
	}
	if (prev) {
		prev->next = cur->next;
	} else {
		obj->first_process = cur->next;
	}

	obj->nof_processes--;
	proc->next = NULL;

	return 0;
}

