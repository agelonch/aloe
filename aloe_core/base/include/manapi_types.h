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

#ifndef MANAPI_TYPES_H_
#define MANAPI_TYPES_H_

#include "str.h"
#include "objects_max.h"
#include "hwapi.h"
#include "manapi_execinfo.h"
#include "manapi_interface.h"
#include "manapi_module.h"
#include "manapi_variable.h"
#include "manapi_waveform.h"

typedef void updateable;


typedef struct {
	float C[MAX(processors)];
	float B[MAX(processors)];
	float I[MAX(processors)][MAX(processors)];
	float C_rem[MAX(processors)];
	float B_rem[MAX(processors)];
} man_platform_model_t;








#endif
