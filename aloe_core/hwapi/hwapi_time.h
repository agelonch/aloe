#ifndef HWAPI_TIME_H
#define HWAPI_TIME_H

#include "hwapi_types.h"

#include "defs.h"


typedef struct {
	/**
	 * Reference time to synchronize timer and indicate the beginning of time.
	 */
	time_t init_time;
	int init_time_is_shm;
	/**
	 * Time-slot length, in microseconds
	 */
	int ts_len_us;
	/**
	 * Time-slot number. Incremented every event of the kernel-thread timer.
	 */
	int ts_counter;
	/**
	 * Time the current timeslot started
	 */
	time_t ts_time;

}hwapi_time_t;

void hwapi_time_set_context(hwapi_time_t *_context);
void hwapi_time_ts_inc();
int hwapi_time_reset();
int hwapi_time_reset_realtime(struct timespec *x);

#endif
