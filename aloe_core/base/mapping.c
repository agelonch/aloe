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

#include "defs.h"
#include "mapping.h"
#include "man_platform.h"
#include "nod_waveform.h"

extern man_platform_t UNUSED(platform);

/**
 * 1) Configure the mapping algorithm
 * 2) generate the c,b waveform model
 * 3) call the mapping algorithm with the man_platform_model() and waveform modules
 * 4) for each module, assign its node.
 * 5) for all modules's interfaces, assign physical interfaces id (if communication is external)
 * 6) construct the result[] array of waveform objects
 */
int mapping_map(nod_waveform_t *waveform) {
	aerror("Not yet implemented");
	return -1;
}
