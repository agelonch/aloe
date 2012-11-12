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

#ifndef NOD_WAVEFORM_H
#define NOD_WAVEFORM_H

#include "objects_max.h"
#include "str.h"
#include "nod_module.h"
#include "nod_waveform_status.h"

/**
 * Used by the node to save the loaded waveforms, its modules and variables. At LOAD command, the class is received and serializedTo() a free Waveform object instance. Then waveform attributes, modules and variables and all their attributes are created on pre-allocated objects instances.
 */
typedef struct {
	nod_module_t modules[MAX(modules)];
	nod_waveform_status_t status;
	/**
	 * Automatically generated waveform id. Must be unique in the platform.
	 */
	int id;
	strdef(name);
} nod_waveform_t;

void nod_waveform_serialize(nod_waveform_t *dest, packet_t *pkt);
void nod_waveform_unserializeTo(packet_t *pkt, nod_waveform_t *dest);

#endif
