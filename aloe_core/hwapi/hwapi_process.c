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

#include <dlfcn.h>
#include "defs.h"
#include "hwapi_error.h"
#include "hwapi_process.h"
#include "hwapi.h"
#include "pipeline.h"

/**
 * Loads a process binary into memory. The process must have been created using
 *  hwapi_process_new(). This function loads the library defined in the process
 *  attributes during the call to hwapi_process_new().
 *  @param obj Pointer to the hwapi_process_t object.
 *  @return Zero on success, -1 on error.
 */
int hwapi_process_launch(hwapi_process_t *obj) {
	HWAPI_ASSERT_PARAM(obj);
	char *error;

	obj->dl_handle = dlopen(obj->attributes.binary_path, RTLD_NOW);

	if (!obj->dl_handle) {
		HWAPI_DLERROR(dlerror());
		return -1;
	}
	dlerror();

	*(void**) (&obj->run_point) = dlsym(obj->dl_handle, "_run_cycle");
	if ((error = dlerror()) != NULL) {
		HWAPI_DLERROR(error);
		return -1;
	}

	*(void**) (&obj->abort_point) = dlsym(obj->dl_handle, "_abort");
	if ((error = dlerror()) != NULL) {
		HWAPI_DLERROR(error);
		return -1;
	}

	return 0;
}

/**
 * Removes a process from the system, previously loaded using hwapi_process_new().
 * hwapi_process_remove() removes the process from the allocated pipeline and
 * then unloads the program code from memory. After, if a finish_callback function
 * was provided in the process attributes during hwapi_process_new(), it is now
 * called.

 *  @param obj Pointer to the hwapi_process_t object.
 *  @return Zero on success, -1 on error.
 *
 * 1) Call hwapi_pipeline_remove(self->pipeline,self);
 * 2) Call to dlclose(handle)
 * 3) Set pid=0
 * 4) if finishCallback!=null, call the function
 */
int hwapi_process_remove(h_proc_t process) {
	HWAPI_ASSERT_PARAM(process);
	hwapi_process_t *obj = (hwapi_process_t*) process;

	if (pipeline_remove((pipeline_t*) obj->pipeline, obj)) {
		return -1;
	}

	dlclose(obj->dl_handle);

	if (obj->attributes.finish_callback) {
		obj->attributes.finish_callback(process);
	}

	obj->pid = 0;

	return -1;
}

/**
 * Returns the id of the process identified by the first argument.
 * The process must have been previously loaded using hwapi_process_new().
 * @param process Handler to the process
 * @returns non-negative id on success, -1 on error
 */
int hwapi_process_pid(h_proc_t process) {
	HWAPI_ASSERT_PARAM(process);
	hwapi_process_t *obj = (hwapi_process_t*) process;
	return obj->pid;
}

/**
 * Enables the execution of the process identified by the first argument.
 * The process must have been previously loaded using hwapi_process_new().
 * @param process Handler to the process
 * @returns zero on success, -1 on error
 */
int hwapi_process_run(h_proc_t process) {
	HWAPI_ASSERT_PARAM(process);
	hwapi_process_t *obj = (hwapi_process_t*) process;
	obj->runnable = 1;
	return 0;
}

/**
 * Disables the execution of the process identified by the first argument.
 * The process must have been previously loaded using hwapi_process_new().
 * @param process Handler to the process
 * @returns zero on success, -1 on error
 */
int hwapi_process_stop(h_proc_t process) {
	HWAPI_ASSERT_PARAM(process);
	hwapi_process_t *obj = (hwapi_process_t*) process;
	obj->runnable = 0;
	return 0;
}


