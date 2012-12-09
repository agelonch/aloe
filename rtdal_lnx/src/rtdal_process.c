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
#include "rtdal_error.h"
#include "rtdal_process.h"
#include "rtdal.h"
#include "pipeline.h"

lstrdef(tmp);

/**
 * Loads a process binary into memory. The process must have been created using
 *  rtdal_process_new(). This function loads the library defined in the process
 *  attributes during the call to rtdal_process_new().
 *  @param obj Pointer to the rtdal_process_t object.
 *  @return Zero on success, -1 on error.
 */
int rtdal_process_launch(rtdal_process_t *obj) {
	hdebug("path=%s\n",obj->attributes.binary_path);
	rtdal_ASSERT_PARAM(obj);
	char *error;

	snprintf(tmp,LSTR_LEN,"/usr/local/lib/%s",obj->attributes.binary_path);
	obj->dl_handle = dlopen(tmp, RTLD_NOW);

	if (!obj->dl_handle) {
		rtdal_DLERROR(dlerror());
		return -1;
	}
	dlerror();

	*(void**) (&obj->run_point) = dlsym(obj->dl_handle, "_run_cycle");
	if ((error = dlerror()) != NULL) {
		rtdal_DLERROR(error);
		return -1;
	}

	return 0;
}

/**
 * Removes a process from the system, previously loaded using rtdal_process_new().
 * rtdal_process_remove() removes the process from the allocated pipeline and
 * then unloads the program code from memory. After, if a finish_callback function
 * was provided in the process attributes during rtdal_process_new(), it is now
 * called.

 *  @param obj Pointer to the rtdal_process_t object.
 *  @return Zero on success, -1 on error.
 *
 * 1) Call rtdal_pipeline_remove(self->pipeline,self);
 * 2) Call to dlclose(handle)
 * 3) Set pid=0
 * 4) if finishCallback!=null, call the function
 */
int rtdal_process_remove(h_proc_t process) {
	rtdal_ASSERT_PARAM(process);
	rtdal_process_t *obj = (rtdal_process_t*) process;
	hdebug("pid=%d\n",obj->pid)

	if (pipeline_remove((pipeline_t*) obj->pipeline, obj)) {
		return -1;
	}

	dlclose(obj->dl_handle);

	obj->pid = 0;

	return 0;
}

/**
 * Returns the id of the process identified by the first argument.
 * The process must have been previously loaded using rtdal_process_new().
 * @param process Handler to the process
 * @returns non-negative id on success, -1 on error
 */
int rtdal_process_pid(h_proc_t process) {
	rtdal_ASSERT_PARAM(process);
	rtdal_process_t *obj = (rtdal_process_t*) process;
	return obj->pid;
}

/**
 * Enables the execution of the process identified by the first argument.
 * The process must have been previously loaded using rtdal_process_new().
 * @param process Handler to the process
 * @returns zero on success, -1 on error
 */
int rtdal_process_run(h_proc_t process) {
	rtdal_ASSERT_PARAM(process);
	rtdal_process_t *obj = (rtdal_process_t*) process;
	hdebug("pid=%d\n",obj->pid);
	obj->runnable = 1;
	return 0;
}

/**
 * Disables the execution of the process identified by the first argument.
 * The process must have been previously loaded using rtdal_process_new().
 * @param process Handler to the process
 * @returns zero on success, -1 on error
 */
int rtdal_process_stop(h_proc_t process) {
	rtdal_ASSERT_PARAM(process);
	rtdal_process_t *obj = (rtdal_process_t*) process;
	hdebug("pid=%d\n",obj->pid);
	obj->runnable = 0;
	return 0;
}

int rtdal_process_seterror(h_proc_t proc, rtdal_processerrors_t code) {
	rtdal_ASSERT_PARAM(proc);
	rtdal_process_t *obj = (rtdal_process_t*) proc;
	hdebug("pid=%d, code=%d\n",obj->pid,(int)code);
	obj->finish_code = code;
	return 0;
}

rtdal_processerrors_t rtdal_process_geterror(h_proc_t proc) {
	rtdal_ASSERT_PARAM(proc);
	rtdal_process_t *obj = (rtdal_process_t*) proc;
	hdebug("pid=%d, code=%d\n",obj->pid,(int)obj->finish_code);
	return obj->finish_code;
}

int rtdal_process_isrunning(h_proc_t proc) {
	rtdal_ASSERT_PARAM(proc);
	rtdal_process_t *obj = (rtdal_process_t*) proc;
	hdebug("pid=%d, running=%d\n",obj->pid,obj->runnable);
	return obj->is_running;
}
