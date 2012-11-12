#include <pthread.h>
#include <sched.h>
#include <unistd.h>
#include <sys/types.h>

#include "defs.h"
#include "hwapi_task.h"
#include "hwapi.h"
#include "hwapi_error.h"


/**
 * Creates a new thread with normal priority, that is, no real-time priority. The thread is detachable, meaning that runs the function pointed by the first parameter and then is destroyed, ignoring the returned value.
 */
int hwapi_task_new(void *(*fnc)(void*), void *arg) {
	return hwapi_task_new_prio(fnc, arg, TASK_DEFAULT_PRIORITY, TASK_DEFAULT_CPUID);
}

/**
 * All threads are created with detachable state.
 */
int hwapi_task_new_prio(void *(*fnc)(void*), void* arg, int prio, int cpu) {
	pthread_t thread;
	return hwapi_task_new_thread(&thread, fnc, arg, DETACHABLE, prio, cpu);
}

int hwapi_task_new_thread(pthread_t *thread, void *(*fnc)(void*), void *arg,
		enum hwapi_task_type type, int prio, int cpu) {
	HWAPI_ASSERT_PARAM(thread);
	HWAPI_ASSERT_PARAM(fnc);
	HWAPI_ASSERT_PARAM(prio>=0);
	HWAPI_ASSERT_PARAM(cpu>=-1);

	pthread_attr_t attr;
	int s,ret;
	struct sched_param param;
	cpu_set_t cpuset;

	ret = -1;

	s = pthread_attr_init(&attr);
	if (s) {
		HWAPI_POSERROR(s, "pthread_attr_init");
		goto destroy_attr;
	}

	if (type == DETACHABLE) {
	       s = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	       if (s) {
		       HWAPI_POSERROR(s, "pthread_attr_setdetachstate");
		       goto destroy_attr;
	       }
	}

	if (cpu != -1) {
		CPU_ZERO(&cpuset);
		CPU_SET((size_t) cpu,&cpuset);
		s =  pthread_attr_setaffinity_np(&attr, sizeof(cpu_set_t), &cpuset);
		if (s) {
			HWAPI_POSERROR(s, "pthread_attr_setaffinity_np");
			goto destroy_attr;
		}
	}

	s = pthread_create(thread, &attr, fnc, arg);
	if (s) {
		if (s == EPERM) {
			awarn("Not enough privileges to set kernel thread priority\n");
		} else {
			HWAPI_POSERROR(s, "pthread_create");
			goto destroy_attr;
		}
	}
	/* try to set priority *AFTER* set creation */
	if (prio != 0) {
		param.sched_priority = prio;
		s = pthread_setschedparam(*thread, SCHED_FIFO, &param);
		if (s == EPERM) {
			awarn("Not enough privileges to set task priority\n");
		} else if (s) {
			HWAPI_POSERROR(s, "pthread_attr_setschedpolicy");
			goto destroy_attr;
		}
	}

	ret=0;

destroy_attr:
	pthread_attr_destroy(&attr);
	return ret;
}
