#ifndef PIPELINE_SYNC_H_
#define PIPELINE_SYNC_H_

#include <pthread.h>
#include <semaphore.h>
#include "futex.h"
#include "objects_max.h"

#define SEM 		1
#define FUTEX 		2
#define BARRIER		3
#define PIPELINESYNC_MUTEX_TYPE	BARRIER

#if PIPELINESYNC_MUTEX_TYPE == SEM
#define pipeline_sync_initialize_ for (int i=0;i<num_pipelines;i++) {\
	sem_init(&semaphores[i], 0, 0); }
#define pipeline_sync_thread_waits(idx) sem_wait(&semaphores[idx])
#define pipeline_sync_threads_wake() for (int i=0;i<num_pipelines;i++) {\
	sem_post(&semaphores[i]); }

static int num_pipelines;
static sem_t semaphores[MAX(processors)];
#endif

#if PIPELINESYNC_MUTEX_TYPE == FUTEX
#define pipeline_sync_initialize_
#define pipeline_sync_thread_waits(idx) futex_wait(&futex)
#define pipeline_sync_threads_wake() futex_wake(&futex)

static int futex;
#endif

#if PIPELINESYNC_MUTEX_TYPE == BARRIER
#define pipeline_sync_initialize_ pthread_barrier_init(&barrier, NULL, \
		(unsigned int) num_pipelines+1)
#define pipeline_sync_thread_waits(idx) pthread_barrier_wait(&barrier)
#define pipeline_sync_threads_wake() pthread_barrier_wait(&barrier)

static pthread_barrier_t barrier;
#endif





#endif
