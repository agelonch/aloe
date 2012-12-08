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

#ifndef HWAPI_PROCESS_H
#define HWAPI_PROCESS_H

#include "str.h"
#include "hwapi_types.h"
#include "hwapi.h"

/**
 * A HWAPI Process is defined as dynamically loaded shared library with two symbols that *MUST* be defined:
 *  - _startup(arg0,arg1): This function is called at load-time and is not subject to real-time constraints.
 *  - _runCycle(): This function is called each time-slot and is subject to real-time constraints.
 * It is used to load and execute signal processing waveform modules.
 * The HWAPI.Process objects are instantiated by Base.initializeNode() function at boot. A call to create() loads the module into memory. A call to startup() calls the _startup() function and a call to places the _runCycle() function into the pipelined execution queue (see Proc-Thread class)
 */
struct _hwapi_process_t {
	int pid;

	void *pipeline;
	void *arg;
	struct hwapi_process_attr attributes;

	void* dl_handle;
	int (*run_point)(void*);
	void (*abort_point)(void*);
	int is_running;
	int runnable;
	hwapi_processerrors_t finish_code;
	strdef(error_msg);
	/* pointer to the next process in the pipeline queue */
	struct _hwapi_process_t *next;
};
typedef struct _hwapi_process_t hwapi_process_t;

int hwapi_process_launch(hwapi_process_t *proc);

#endif
