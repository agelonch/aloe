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

#ifndef MANAPI_VARIABLE_H_
#define MANAPI_VARIABLE_H_

/**
 * Inherits from General.ModVariable. Part of the CMDAPI. Adds functionality to set or obtain variable values from the distributed modules.
 */
typedef struct {
	/**
	 * Unique id in the waveform
	 */
	int id;
	/**
	 * Size of the variable, in bytes
	 */
	int size;
	/**
	 * Name of the variable, as it is created by the module or defined in the .app. Note the module must create its variable with the same name as defined in the .app if wants to use the initialization value.
	 */
	strdef(name);

	/**
	 * Number of samples to capture from the variable during variable reporting. If the variable has len bytes, window*len bytes will be captured each period timeslots.
	 */
	int window;
	/**
	 * Period, in timeslots, the variable value is reported.
	 */
	int period;
	int periodCnt;
	void* cur_value;

} variable_t;


#endif
