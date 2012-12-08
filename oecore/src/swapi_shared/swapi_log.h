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

#ifndef SWAPI_LOG_H
#define SWAPI_LOG_H

#include "str.h"
#include "swapi.h"

/**
 * A Log is a mechanism for the module to write text to a logfile without interrupting real-time processing. Successive calls to Log() or Write() functions store the string in a temporal buffer. At the end of the timeslot, a new thread with lower priority is created to write the contents a non-volatile media provided by the hwapi (e.g. a hard-drive or flash-rom). Note that it may happen that the contents are never written, in the case that the processor is 100% of its time running signal processing tasks.
 */
typedef struct {
	int id;
	strdef(name);
	lstrdef(buffer);
	int fd;
	int w_ptr;
	void *context;
}swapi_log_t;

int swapi_log_init(void *context, swapi_log_t *log, string name);

#endif
