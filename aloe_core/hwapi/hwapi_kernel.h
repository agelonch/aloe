#ifndef HWAPI_KERNEL_H
#define HWAPI_KERNEL_H

#define THREADED_CPU

#define ALOE_VERSION "ALOE++-0.1-alpha"

#ifdef THREADED_CPU
#define CORE_ID(i) 2*i+1
#else
#define CORE_ID(i) i
#endif

void hwapi_kernel_tasks();
int hwapi_kernel_sigwait_thread();

#endif
