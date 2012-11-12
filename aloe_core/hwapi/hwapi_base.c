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

#include <stddef.h>
#include "defs.h"
#include "str.h"
#include "objects_max.h"
#include "hwapi_context.h"
#include "hwapi_time.h"
#include "hwapi_file.h"
#include "hwapi_error.h"
#include "hwapi.h"

static hwapi_context_t *context;

/**
 * Must be called before any other function.
 *
 * 1) Parse configuration file and fill +machine attributes
 * 2) Create interfaces. Call -createNodeInterfaces()
 * 3) Setup synchronization:
 *   3.1) If I am the synchronization master, set machine.clockSource=CLOCKSRC_TIMER if source="timer" or machine.clockSource=CLOCKSRC_DAC if source="dac:/"
 *   3.2) If I am an slave node, set machine.clockSource=CLOCKSRC_TIMER if continuous_mode=FALSE or machine.clockSource=CLOCKSRC_SYNC if continuous_mode=TRUE
 *   3.3) Set machine.syncContinuous boolean if continuous_mode=TRUE
 *   3.4) Set machine.syncMaster boolean if I am the synchronization master reference
 *   3.5) Set machine.syncPeriod to the synchronization period
 * 4) setup DAC (if my node has a DAC). For each DAC, create an instance of the DAC type and:
 *   4.1) Call DAC.openDAC(address,options,null) if machine.clockSource!=CLOCKSRC_DAC
 *   4.2) Call DAC.setScheduler(tsBeginFnc, machine.kernelPrio) if machine.clockSource==CLOCKSRC_DAC
 * 5) create max_queues_x_node +queues and max_modules_x_node +procs objects
 */
int hwapi_initialize_node(hwapi_context_t *_context, string config_file,
		void (*ts_begin_fnc)(void) ) {
	assert(_context);

	context = _context;

	hwapi_error_set_context(&context->error);
	context->time.ts_len_us = context->machine.ts_len_us;
	hwapi_time_set_context(&context->time);
	//hwapi_file_set_path("outputs");
	hwapi_time_reset();

	return -1;
}


/**
 * 1) PhysicItf.create("tcp://local_ip:ctrl_port") mode in name "ctrl"
 * 2) PhysicItf.create("udp://local_ip:sync_port") mode in name "sync"
 * 3) PhysicItf.create("udp://local_ip:probe_port") mode in name "probe"
 * 4) If !multicast, PhysicItf.create("udp://node_ip:sync_port") mode out name "slavesync%d" for each n in node list
 * 5) If multicast, PhysicItf.create("udp://MULTICAST_GROUP1:sync_port") mode out name "slavesync0" (only one output interface is created)
 * 6) Create data interfaces with name "data_id" with id consecutive numbers
 */
static int __attribute__((__unused__)) start_node_interfaces(string config_file) {
	assert(context);
	aerror("Not yet implemented");
	return -1;
}

/**
 * Configures the control, probe and sync interfaces for the manager
 * 1) For each node, create an interface with name ctrl_nodename
 * 2) For each node, create an interface with name probe_nodename
 * 3) if clockLocal create the sync interface to the sync master, with name "sync"
 */
int hwapi_start_manager_interfaces(string config_file) {
	assert(context);
	aerror("Not yet implemented");
	return -1;
}







/**
 * Adds the function callback to the kernel periodic callback functions.
 * If hwapi_periodic_add() successes, the function callback will be called every
 * period timeslots with Kernel Priority.
 *
 * @param callback Pointer to the periodic function
 * @param period Positive integer, in time slots
 * @return zero on success, -1 on error
 */
int hwapi_periodic_add(void (*callback)(void), int period) {
	assert(context);
	HWAPI_ASSERT_PARAM(callback);
	HWAPI_ASSERT_PARAM(period>0);

	int i;

	for (i=0;i<MAX(hwapi_periodic);i++) {
		if (!context->periodic[i].callback)
			break;
	}
	if (i == MAX(hwapi_periodic)) {
		HWAPI_SETERROR(HWAPI_ERROR_NOSPACE);
		return -1;
	}
	context->periodic[i].counter = 0;
	context->periodic[i].period = period;
	context->periodic[i].callback = callback;

	return 0;
}

/**
 * Removes the function pointed by callback from the kernel periodic callback functions,
 * previously added with hwapi_periodic_add().
 *
 * @param callback Pointer to the periodic function
 * @returns zero on success, -1 on error
 */
int hwapi_periodic_remove(void (*callback)(void)) {
	assert(context);
	HWAPI_ASSERT_PARAM(callback);

	int i;

	for (i=0;i<MAX(hwapi_periodic);i++) {
		if (context->periodic[i].callback == callback)
			break;
	}
	if (i == MAX(hwapi_periodic)) {
		HWAPI_SETERROR(HWAPI_ERROR_NOTFOUND);
		return -1;
	}
	context->periodic[i].counter = 0;
	context->periodic[i].period = 0;
	context->periodic[i].callback = NULL;

	return 0;
}

/**
 * Blocks the caller until the time slot wake_tslot. The function can not be called
 * again until it returs to the original caller.
 *
 * @param wake_tslot positive integer indicating the time slot to wake up
 * @return 0 on success, -1 on error
 *
 */
int hwapi_sleep(int wake_tslot) {
	assert(context);
	HWAPI_ASSERT_PARAM(wake_tslot>=0);

	if (context->wake_tslot > 0) {
		HWAPI_ERROR("Already being blocked");
		return -1;
	}

	context->wake_tslot = wake_tslot;
	sem_wait(&context->sleep_semaphore);
	context->wake_tslot = 0;

	return 0;
}

/**
 * Returns a handler to the first physical interface matching the name.
 * @param name Name of the physical interface
 * @return non-null value on success, zero on error
 */
h_phyitf_t hwapi_itf_physic_get(string name) {
	assert(context);
	HWAPI_ASSERT_PARAM_P(name);
	int i;

	for (i=0;i<MAX(hwapi_itfphysic);i++) {
		if (!strcmp(context->physic_itfs[i].parent.name, name))
			break;
	}
	if (i == MAX(hwapi_itfphysic)) {
		HWAPI_SETERROR(HWAPI_ERROR_NOTFOUND);
		return NULL;
	}

	return (h_phyitf_t) &context->physic_itfs[i];
}

/**
 * Creates a new message queue.
 * @param name Name of the new queue interface
 * @param max_msg Positive integer. Maximum number of messages that can be inserted in the queue
 * @param msg_sz Positive integer. Maximum message size
 * @return non-null value on success, zero on error
 */
h_qitf_t hwapi_itfqueue_new(string name, int max_msg, int max_msg_sz) {
	assert(context);
	HWAPI_ASSERT_PARAM_P(name);
	HWAPI_ASSERT_PARAM_P(max_msg>=0);
	HWAPI_ASSERT_PARAM_P(max_msg_sz>=0);

	int i;

	for (i=0;i<MAX(hwapi_itfqueue);i++) {
		if (!context->queues[i].parent.id)
			break;
	}
	if (i == MAX(hwapi_itfqueue)) {
		HWAPI_SETERROR(HWAPI_ERROR_NOTFOUND);
		return NULL;
	}

	context->queues[i].max_msg = max_msg;
	context->queues[i].max_msg_sz = max_msg_sz;

	if (hwapi_itfqueue_init(&context->queues[i])) {
		return NULL;
	}

	context->queues[i].parent.id = i+1;
	strncat(context->queues[i].parent.name, name, STR_LEN);

	return (h_qitf_t) &context->queues[i];
}

/**
 * Returns a handler to the first queue interface matching the name.
 * @param name Name of the queue interface
 * @return non-null value on success, zero on error
 */
h_qitf_t hwapi_itfqueue_get(string name) {
	assert(context);
	HWAPI_ASSERT_PARAM_P(name);
	int i;

	for (i=0;i<MAX(hwapi_itfqueue);i++) {
		if (context->queues[i].parent.id) {
			if (!strcmp(context->queues[i].parent.name, name))
				break;
		}
	}
	if (i == MAX(hwapi_itfqueue)) {
		HWAPI_SETERROR(HWAPI_ERROR_NOTFOUND);
		return NULL;
	}

	return (h_qitf_t) &context->queues[i];
}

/**
 * Returns a handler to the first dac object matching the name.
 * @param name Name of the dac
 * @return non-null value on success, zero on error
 */
h_dac_t hwapi_dac_get(string address) {
	assert(context);
	aerror("Not yet implemented");
	return NULL;
}


/**
 * Creates a new process to be executed in one of the kernel pipelines.
 * The structure pointed by attr has the following options:
 *  - binary_path: Path to the shared library where the program code resides
 *  - pipeline_id: If the machine has more than one pipeline (e.g. in a multi-core
 *  system, it indicates to which the process will be loaded).
 *  - exec_position: Indicates the execution position in the pipeline.
 *  - finish_callback: If non-null, the kernel will call this function when the
 *  process finishes (normally or due to an error) its execution.
 *
 * hwapi_process_new() configures the process parameters and loads it into the
 * pipeline. The process won't execute until a success call to hwapi_process_run()
 * is performed.
 *
 * The address pointed by arg will be passed to the process as a parameter.
 *
 * Notes: The shared library must conform with the hwapi_process_t requirements.
 *
 * @param attr Configures the process attributes
 * @param arg Argument to pass to the process each execution cycle
 * @return non-null value on success, zero on error
 */
h_proc_t hwapi_process_new(struct hwapi_process_attr *attr, void *arg) {
	assert(context);
	HWAPI_ASSERT_PARAM_P(attr);
	HWAPI_ASSERT_PARAM_P(arg);
	int i=0;

	/* find empty space on process db */
	for (i=0;i<MAX(hwapi_process);i++) {
		if (!context->processes[i].pid)
			break;
	}
	if (i == MAX(hwapi_process)) {
		HWAPI_SETERROR(HWAPI_ERROR_NOSPACE);
		return NULL;
	}

	context->processes[i].pid=i+1;
	memcpy(&context->processes[i].attributes, attr,
			sizeof(struct hwapi_process_attr));
	context->processes[i].arg = arg;

	if (pipeline_add(&context->pipelines[attr->pipeline_id],
			&context->processes[i]) == -1) {
		return NULL;
	}

	if (hwapi_process_launch(&context->processes[i])) {
		pipeline_remove(&context->pipelines[attr->pipeline_id],
				&context->processes[i]);
		return NULL;
	}

	return (h_proc_t) &context->processes[i];
}

