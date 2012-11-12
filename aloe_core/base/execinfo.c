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


#include "modexecinfo.h"

/**
 * Updates the local object's attributes with the node's attributes.
 * 1) Verify mmodule.waveform.Status.isLoaded()
 * 2) Send Cmd.GET dest(module.waveform.Id, module.Id,0) to module.node using module.waveform.cmdPlatform
 * 3) wait for answer and serializeTo(module.execInfo)
 * 4) update lastUpdateTS with current ts
 */
int execinfo_update(execinfo_t *obj) {
	aerror("Not yet implemented");
	
	return -1;
}

void execinfo_serialize(execinfo_t *src, packet_t *pkt) {
	aerror("Not yet implemented");
}

void execinfo_unserializeTo(packet_t *pkt, execinfo_t *dest) {
	aerror("Not yet implemented");
}
