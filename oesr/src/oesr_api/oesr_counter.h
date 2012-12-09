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

#ifndef oesr_COUNTER_H
#define oesr_COUNTER_H

#include "str.h"
#include "defs.h"
#include "rtdal.h"

/**
 * A Counter is a class used to count short periods of time in a timeslot execution cycle, for instance, to measure the execution time of some parts of the code. It is associated with a public variable the name passed to the create() function. After a call to Stop(), the number of microseconds elapsed since the previous call to Start() is saved in the variable.
 */
typedef struct {
	int id;
	time_t count[3];
	void *variable;
	void *context;
}oesr_counter_t;

int oesr_counter_init(void *context, oesr_counter_t *counter, string name);

#endif
