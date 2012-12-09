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
#include "man_platform.h"
#include "man_probelistener.h"

int man_probelistener_add(man_probelistener_t *lstnr, void (*callback)(void), int code) {
	aerror("Not yet implemented");
	return -1;
}

int man_probelistener_remove(man_probelistener_t *lstnr, int code) {
	aerror("Not yet implemented");
	return -1;
}

/**
 * Sets process() function as a callback to the interface
 *  
 */
int man_probelistener_setup(man_probelistener_t *lstnr, r_itf_t *itf) {
	aerror("Not yet implemented");
	return -1;
}

/**
 * Called when a new packet arrives, receives it from the interface and uses the CmdPacket interface to read the first 32-bit word of the packet, find the associated handler and call it, passing the packet data pointer and size as first and second parameters.
 */
int man_probelistener_process(man_probelistener_t *lstnr) {
	aerror("Not yet implemented");
	return -1;
}
