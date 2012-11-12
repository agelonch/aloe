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

#ifndef NOD_WAVEFORM_STATUS
#define NOD_WAVEFORM_STATUS

#include "waveform_status.h"
#include "packet.h"

typedef struct {
	waveform_status_t parent;
} nod_waveform_status_t;

void nod_waveform_status_serialize(nod_waveform_status_t *src, packet_t *pkt);
void nod_waveform_status_unserializeTo(packet_t *pkt, nod_waveform_status_t *dest);

#endif

