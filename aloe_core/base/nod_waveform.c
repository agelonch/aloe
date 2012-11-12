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

#include "nod_waveform.h"

/**
 * Save attributes (and status) and call waveform.moduleObj[i].serialize() for all modules
 */
void nod_waveform_serialize(nod_waveform_t *src, packet_t *pkt) {
	aerror("Not yet implemented");
}

/**
 * Save attributes to the object instance and call waveform.moduleObj[i].unserializeTo() all nof_modules module instances
 */
void nod_waveform_unserializeTo(packet_t *pkt, nod_waveform_t *dest) {
	aerror("Not yet implemented");
}
