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

#ifndef MAPPING_H
#define MAPPING_H

#include "objects_max.h"
#include "waveform.h"

/**
 * This class runs the waveform-to-platform mapping algorithm and stores the result. The mapping procedure is as follows:
 * 1) Create an object instance
 * 2) Setup algorithm and options
 * 3) call to map(platform,waveform) to map "waveform" object to "platform" object
 * 4) Result is stored in p_res and cost fields
 */

typedef struct {
	int *p_res;
	int modules_x_node[MAX(processors)];
	float cost;
	int algorithm;
	int options;

} mapping_t;

int mapping_map(mapping_t *m, waveform_t *waveform);

#endif
