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

#ifndef rtdal_ITF_H
#define rtdal_ITF_H

#include "str.h"
#include "rtdal.h"

/**
 * Abstract class that manages the rtdal data or control, physical or logical interfaces.
 */
typedef struct {
	int id;
	int is_external;

	strdef(name);
	int mode_is_input;
	/**
	 * Interface delay, in timeslots
	 */
	int delay;
	int is_blocking;
	void (*callback)(void);

}rtdal_itf_t;

#endif
