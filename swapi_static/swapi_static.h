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

#ifndef SWAPI_STATIC_H
#define SWAPI_STATIC_H

/**
 * This class has to be linked statically with any ALOE waveform module.
 */

/**
 * This is an internal SWAPI function, called by the hwapi_processs each timeslot. It does not follow the public method naming convention.
 * This function manages the status state machine, computes execution time statistics, writes log files and produces variable reports. The status STOP and INIT are executed only once in non real-time priority.
 */
int _run_cycle(void* context);

/**
 * called from the kernel or the swapi due to some aerror. Set mymodule.status=STOP and call sStop()
 */
void _abort(void* context);

int Run(void *context);

#endif
