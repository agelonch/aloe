#ifndef HWAPI_TASK_H_
#define HWAPI_TASK_H_

#define TASK_DEFAULT_PRIORITY 0
#define TASK_DEFAULT_CPUID 0

enum hwapi_task_type {
	DETACHABLE, JOINABLE
};

int hwapi_task_new_thread(pthread_t *thread, void *(*fnc)(void*), void *arg, \
		enum hwapi_task_type type, int prio, int cpu);

#endif
