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

#include <pthread.h>
#include <sched.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>

#include "defs.h"
#include "rtdal_task.h"
#include "rtdal.h"
#include "rtdal_error.h"
#include "rtdal_kernel.h"

extern int *core_mapping;
extern int nof_cores;


/**
 * Creates a new thread with normal priority, that is, no real-time priority. The thread is detachable, meaning that runs the function pointed by the first parameter and then is destroyed, ignoring the returned value.
 */
int rtdal_task_new(h_task_t *task, void *(*fnc)(void*), void *arg) {
	return rtdal_task_new_prio(task, fnc, arg, TASK_DEFAULT_PRIORITY, TASK_DEFAULT_CPUID);
}

int rtdal_task_kill(h_task_t task) {
	int s = pthread_kill((pthread_t) task,TASK_TERMINATION_SIGNAL);
	if (s != 0) {
		rtdal_POSERROR(s, "pthread_kill");
		return -1;
	}
	return 0;
}

int rtdal_task_wait(h_task_t task, void **retval) {
	hdebug("task=%d\n",task);
	int s = pthread_join((pthread_t) task,retval);
	if (s != 0) {
		rtdal_POSERROR(s, "pthread_join");
		return -1;
	}
	return 0;
}

int rtdal_task_wait_nb(h_task_t task, void **retval) {
	hdebug("task=%d\n",task);
	int s = pthread_tryjoin_np((pthread_t) task,retval);
	if (s != 0) {
		if (s == EBUSY) {
			return 0;
		} else {
			rtdal_POSERROR(s, "pthread_join");
			return -1;
		}
	}
	return 1;
}

/**
 * All threads are created with detachable state.
 */
int rtdal_task_new_prio(h_task_t *task, void *(*fnc)(void*), void* arg, int prio, int cpu) {
	enum rtdal_task_type type;
	pthread_t thread, *tmp;
	int unblock_signals;

	if (task == NULL) {
		tmp = &thread;
		type = DETACHABLE;
		unblock_signals = 0;
	} else {
		tmp = task;
		type = JOINABLE;
		unblock_signals = 1;
	}
	return rtdal_task_new_thread(tmp, fnc, arg, type, prio, cpu, unblock_signals);
}

int rtdal_task_new_thread(pthread_t *thread, void *(*fnc)(void*), void *arg,
		enum rtdal_task_type type, int prio, int cpu, int unblock_signals) {
	hdebug("thread=0x%x,fnc=0x%x,arg=0x%x,type=%s,prio=%d,cpu=%d\n",thread,fnc,arg,
			type==DETACHABLE?"detach":"join",prio,cpu);
	rtdal_ASSERT_PARAM(thread);
	rtdal_ASSERT_PARAM(fnc);
	rtdal_ASSERT_PARAM(prio>=0);
	rtdal_ASSERT_PARAM(cpu>=-1);

	pthread_attr_t attr;
	int s,ret;
	struct sched_param param;
	cpu_set_t cpuset;
	int i;

	ret = -1;

	s = pthread_attr_init(&attr);
	if (s) {
		rtdal_POSERROR(s, "pthread_attr_init");
		goto destroy_attr;
	}

	s = pthread_attr_setinheritsched(&attr,PTHREAD_EXPLICIT_SCHED);
	if (s) {
		rtdal_POSERROR(s,"pthread_attr_setinheritsched");
		goto destroy_attr;
	}

	s = pthread_attr_setdetachstate(&attr,
			(type == DETACHABLE)?PTHREAD_CREATE_DETACHED:PTHREAD_CREATE_JOINABLE);
	if (s) {
	   rtdal_POSERROR(s, "pthread_attr_setdetachstate");
	   goto destroy_attr;
	}

	if (cpu != -1) {
		CPU_ZERO(&cpuset);
		CPU_SET((size_t) cpu,&cpuset);
	} else {
		CPU_ZERO(&cpuset);
		for (i=0;i<nof_cores;i++) {
			CPU_SET((size_t) core_mapping[i],&cpuset);
		}
	}
	s =  pthread_attr_setaffinity_np(&attr, sizeof(cpu_set_t), &cpuset);
	if (s) {
		rtdal_POSERROR(s, "pthread_attr_setaffinity_np");
		goto destroy_attr;
	}

	if (!getuid()) {
		s = pthread_attr_setschedpolicy(&attr, (prio==TASK_DEFAULT_PRIORITY)?SCHED_RR:SCHED_FIFO);
		if (s) {
			if (s == EPERM) {
				awarn("Not enough privileges to set task scheduling\n");
			} else {
				rtdal_POSERROR(s,"pthread_attr_setschedpolicy");
				goto destroy_attr;
			}
		}
	}

	if (!getuid()) {
		param.sched_priority = prio;
		s = pthread_attr_setschedparam(&attr, &param);
		if (s) {
			if (s == EPERM) {
				awarn("Not enough privileges to set task priority\n");
			} else {
				printf("prio=%d\n",prio);
				rtdal_POSERROR(s,"pthread_attr_setschedparam");
				goto destroy_attr;
			}
		}
	}

	s = pthread_create(thread, &attr, fnc, arg);
	if (s) {
		if (s == EPERM) {
			awarn("Not enough privileges to set kernel thread priority\n");
		} else {
			rtdal_POSERROR(s, "pthread_create");
			goto destroy_attr;
		}
	}

	ret=0;

destroy_attr:
	pthread_attr_destroy(&attr);
	return ret;
}

void rtdal_task_print_sched() {
	pthread_t thread;
	cpu_set_t cpuset;
	struct sched_param param;
	int policy;
	const char *p;
	int s,j;
	int detachstate;

	thread = pthread_self();

	s = pthread_getaffinity_np(thread, sizeof(cpu_set_t), &cpuset);
	if (s != 0) {
		printf("error pthread_getaffinity_np: %s\n",strerror(s));
	}

	printf("Set returned by pthread_getaffinity_np() contained:\n");
	for (j = 0; j < CPU_SETSIZE; j++)
		if (CPU_ISSET(j, &cpuset))
			printf("    CPU %d\n", j);

	s = pthread_getschedparam(thread, &policy, &param);
	if (s != 0) {
		printf("error pthread_getaffinity_np: %s\n",strerror(s));
	}

	switch(policy) {
	case SCHED_FIFO:
		p = "SCHED_FIFO";
		break;
	case SCHED_RR:
		p = "SCHED_RR";
		break;
	default:
		p = "Other";
		break;
	}

	printf("Sched policy is %s. Priority is %d\n",p,param.sched_priority);

}
