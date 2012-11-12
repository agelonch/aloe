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

#include <time.h>
#include <sys/time.h>
#include <errno.h>

#include "defs.h"
#include "defs.h"
#include "hwapi_time.h"
#include "hwapi.h"
#include "hwapi_error.h"

static hwapi_time_t *context = NULL;

/**
 * Sets the hwapi context pointer
 * @param _context
 */
void hwapi_time_set_context(hwapi_time_t *_context) {
	assert(_context);
	context = _context;
}

int hwapi_time_reset() {
	assert(context);

	struct timespec x;

	if (clock_gettime(CLOCK_MONOTONIC,&x)) {
		HWAPI_SYSERROR("clock_gettime");
		return -1;
	}
	context->init_time.tv_sec = x.tv_sec;
	context->init_time.tv_usec = x.tv_nsec/1000;
	context->ts_counter = 0;
	return 0;
}


int hwapi_time_reset_realtime(struct timespec *x) {
	assert(context);

	context->init_time.tv_sec = x->tv_sec;
	context->init_time.tv_usec = x->tv_nsec/1000;
	context->ts_counter = 0;
	return 0;
}

inline void hwapi_time_ts_inc() {
	context->ts_counter++;
}

/**
 * This function is called by syncSlave() in TWO-WAY synchronization mode.
 * It adjusts the local time-slot timer. It has an effect only if
 * clockSource==CLOCKSRC_TIMER
 */
int hwapi_time_set(time_t *time) {
	assert(context);
	HWAPI_ASSERT_PARAM(time);

	time_t tact;
	struct timespec x;

	if (clock_gettime(CLOCK_MONOTONIC,&x)) {
		HWAPI_SYSERROR("clock_gettime");
		return -1;
	}
	tact.tv_sec = x.tv_sec;
	tact.tv_usec = x.tv_nsec/1000;
	timersub(&tact, time, &context->init_time);
	return 0;
}

/**
 * Stores the current time in the structure pointed by time pointer.
 */
int hwapi_time_get(time_t *time) {
	assert(context);
	HWAPI_ASSERT_PARAM(time);

	time_t tact;
	struct timespec x;

	if (clock_gettime(CLOCK_MONOTONIC,&x)) {
		HWAPI_SYSERROR("clock_gettime");
		return -1;
	}
	tact.tv_sec = x.tv_sec;
	tact.tv_usec = x.tv_nsec/1000;
	timersub(&tact, &context->init_time, time);

	return 0;
}

/**
 * returns the time-slot number
 */
int hwapi_time_slot() {
	assert(context);

	return context->ts_counter;
}

/**
 * tdata points to a buffer of 3 time_t consecutive structures.
 * Computes the time interval between position 1 and position 2, storing
 * the result in position 0
 * @param tdata
 */
void hwapi_time_interval(time_t * tdata) {
	assert(tdata);

	tdata[0].tv_sec = tdata[2].tv_sec - tdata[1].tv_sec;
	tdata[0].tv_usec = tdata[2].tv_usec - tdata[1].tv_usec;
	if (tdata[0].tv_usec < 0) {
		tdata[0].tv_sec--;
		tdata[0].tv_usec += 1000000;
	}
}

/**
 * Converts a given time to the equivalent timestamp, given the current slot
 * length
 * @param tdata
 * @return
 */
int hwapi_time_to_tstamp(time_t *tdata) {
	assert(context);
	HWAPI_ASSERT_PARAM(tdata);

	long long int x;

	x = ((long long int) tdata->tv_sec)*1000000 + tdata->tv_usec;
	return (int) (x/(context->ts_len_us));
}

/**
 * Converts a timestamp to the equivalent time at its origin, given the platform
 * timeslot length
 * @param tstamp
 * @param tdata
 */
void tstamp_to_time(int tstamp, time_t *tdata) {
	assert(context);
	assert(tdata);

	long long int x;

	x = ((long long int) tstamp * (context->ts_len_us));
	tdata[0].tv_sec = (int) (x/ 1000000);
	tdata[0].tv_usec = (int) (x - tdata[0].tv_sec * 1000000);
}


/**
 * Called by the manager if obtains the time from shared memory.
 * 1) call shm_open("/initTime"); and save pointer to init_time.
 * 2) set initTimeShm=TRUE
 */
int hwapi_time_attach_local() {
	aerror("Not yet implemented");
	return -1;
}
