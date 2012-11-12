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

#ifndef NOD_VARIABLE_H
#define NOD_VARIABLE_H

#include "variable.h"
#include "packet.h"

/**
 * A module variable is a variable that can be viewed (get) or modified (set) by the CMDAPI or any other module of the same waveform. This interaction is always *asynchronous* and there is no guarantee in terms of timings.
 * 
 * An initialization value can be defined for a variable. In that case, it is specified in the .app file and sent to the Nodes during the waveform load phase. The waveform modules can restore their variables to the initialization value any time.
 * 
 * Those variables which are not specified in the .app are not available to the manager (i.e. there is any ModuleVariable object associated to the Module object) until the module calls the SWAPI.InitVariable() function and the manager calls the moduleObj.update() or waveformObj.update() functions. It is recommended that the modules initialize their variables during INIT() and the manager calls waveformObj.update() after a successful INIT.
 * 
 * This class is used by ALOE-Node. The CMDAPI uses the CmdVariable, which inherits from this class.
 */
typedef struct {
	variable_t parent;
	packet_t report_packet;
	/**
	 * Pointer to the shared memory region where the variable is stored.
	 */
	void* shm_addr;
} nod_variable_t;

void nod_variable_serialize(nod_variable_t *src, packet_t *pkt);
void nod_variable_unserializeTo(packet_t *pkt, nod_variable_t *dest);

#endif
