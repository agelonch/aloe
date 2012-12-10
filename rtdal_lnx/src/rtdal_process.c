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

extern int pgroup_notified_failure[MAX_PROCESS_GROUP_ID];

/**
 * Loads a process binary into memory. The process must have been created using
 *  rtdal_process_new(). This function loads the library defined in the process
 *  attributes during the call to rtdal_process_new().
 *  @param obj Pointer to the rtdal_process_t object.
 *  @return Zero on success, -1 on error.
 */
int rtdal_process_launch(rtdal_process_t *obj) {
	hdebug("path=%s\n",obj->attributes.binary_path);
	RTDAL_ASSERT_PARAM(obj);
	char *error;

	snprintf(tmp,LSTR_LEN,"/usr/local/lib/%s",obj->attributes.binary_path);
	obj->dl_handle = dlopen(tmp, RTLD_NOW);

	if (!obj->dl_handle) {
		RTDAL_DLERROR(dlerror());
		return -1;
	}
	dlerror();

	*(void**) (&obj->run_point) = dlsym(obj->dl_handle, "_run_cycle");
	if ((error = dlerror()) != NULL) {
		RTDAL_DLERROR(error);
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
 */
int rtdal_process_remove(r_proc_t process) {
	RTDAL_ASSERT_PARAM(process);
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
int rtdal_process_pid(r_proc_t process) {
	RTDAL_ASSERT_PARAM(process);
	rtdal_process_t *obj = (rtdal_process_t*) process;
	return obj->pid;
}

/**
 * Enables the execution of the process identified by the first argument.
 * The process must have been previously loaded using rtdal_process_new().
 * @param process Handler to the process
 * @returns zero on success, -1 on error
 */
int rtdal_process_run(r_proc_t process) {
	RTDAL_ASSERT_PARAM(process);
	rtdal_process_t *obj = (rtdal_process_t*) process;
	hdebug("pid=%d\n",obj->pid);
	obj->runnable = 1;
	return 0;
}

/**
 * Disables the execution of the process identified by the first argument.
 * The process must have been previously loaded using rtdal_process_new().
 * @param process Process handler given by rtdal_process_new()
 * @returns zero on success, -1 on error
 */
int rtdal_process_stop(r_proc_t process) {
	RTDAL_ASSERT_PARAM(process);
	rtdal_process_t *obj = (rtdal_process_t*) process;
	hdebug("pid=%d\n",obj->pid);
	obj->runnable = 0;
	return 0;
}

/**
 * Sets an error code for a process.
 * \param proc Process handler given by rtdal_process_new()
 * \returns zero on success, -1 on error
 */
int rtdal_process_seterror(r_proc_t proc, rtdal_processerrors_t code) {
	RTDAL_ASSERT_PARAM(proc);
	rtdal_process_t *obj = (rtdal_process_t*) proc;
	hdebug("pid=%d, code=%d\n",obj->pid,(int)code);
	obj->finish_code = code;
	return 0;
}


/**
 * Returns the process error.
 * \param proc Process handler given by rtdal_process_new()
 * \returns process error code or -1 on error
 */
rtdal_processerrors_t rtdal_process_geterror(r_proc_t proc) {
	RTDAL_ASSERT_PARAM(proc);
	rtdal_process_t *obj = (rtdal_process_t*) proc;
	hdebug("pid=%d, code=%d\n",obj->pid,(int)obj->finish_code);
	return obj->finish_code;
}


/**
 * Returns 1 if the process is running or zero otherwise
 */
int rtdal_process_isrunning(r_proc_t proc) {
	RTDAL_ASSERT_PARAM(proc);
	rtdal_process_t *obj = (rtdal_process_t*) proc;
	hdebug("pid=%d, running=%d\n",obj->pid,obj->runnable);
	return obj->is_running;
}

/**
 * Acknowledges that the process group error notification has been processed, enabling another
 * future call to the finish_callback function.
 * \returns 0 on success, -1 on error
 */
int rtdal_process_group_notified(r_proc_t proc) {
	RTDAL_ASSERT_PARAM(proc);
	rtdal_process_t *obj = (rtdal_process_t*) proc;
	hdebug("pid=%d, running=%d\n",obj->pid,obj->runnable);
	if (obj->attributes.process_group_id < 0 || obj->attributes.process_group_id > MAX_PROCESS_GROUP_ID) {
		RTDAL_SETERROR(RTDAL_ERROR_INVAL);
		return -1;
	}
	pgroup_notified_failure[obj->attributes.process_group_id] = 0;
	return 0;
}
