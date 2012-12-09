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

#ifndef rtdal_TIME_H
#define rtdal_TIME_H

#include "rtdal_types.h"

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

}rtdal_time_t;

void rtdal_time_set_context(rtdal_time_t *_context);
void rtdal_time_ts_inc();
int rtdal_time_reset();
int rtdal_time_reset_realtime(struct timespec *x);

#endif
