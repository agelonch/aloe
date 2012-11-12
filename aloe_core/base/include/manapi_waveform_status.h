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

#ifndef MANAPI_WAVEFORM_STATUS_H_
#define MANAPI_WAVEFORM_STATUS_H_

typedef enum {
	PARSED, LOADED, INIT, RUN, PAUSE, STOP
} waveform_status_enum;

typedef struct {
	waveform_status_enum cur_status;
	int next_timeslot;
	/**
	 * Indicates the deadline timeslot to check if the status has been changed correctly.
	 */
	int dead_timeslot;
} waveform_status_t;



#endif
