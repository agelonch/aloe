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


#ifndef MANAPI_INTERFACE_H_
#define MANAPI_INTERFACE_H_

#include "str.h"

typedef struct {
	/**
 * Data interface between modules.
 */

	/**
	 * Unique id in the waveform
	 */
	int id;
	/**
	 * interface bandwidth requirements in MBPTS
	 */
	float bw_total_mbpts;
	/**
	 * Used bandwidth (in MBPTS). *NOTE*: Unlike module's ExecInfo, the interfaces are assumed to use constant bandwidth.
	 */
	float bw_use_mbpts;
	/**
	 * Name of the interface
	 */
	strdef(name);
	strdef(remoteItfName);
	strdef(remoteObjName);
	int remoteItfId;
	int remoteObjId;
	int phyItfId;
	/**
	 * Interface delay, in timeslots
	 */
	int delay;
	int active;

} interface_t;

#endif
