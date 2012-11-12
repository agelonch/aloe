#ifndef HWAPI_PROCESS_H
#define HWAPI_PROCESS_H

#include "str.h"
#include "hwapi_types.h"

typedef enum {
	OK, RTFAULT, SIG_RECV, RUNERROR
}hwapi_processerrors_t;

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
	int runnable;
	hwapi_processerrors_t finish_code;
	strdef(error_msg);
	/* pointer to the next process in the pipeline queue */
	struct _hwapi_process_t *next;
};
typedef struct _hwapi_process_t hwapi_process_t;

int hwapi_process_launch(hwapi_process_t *proc);

#endif
