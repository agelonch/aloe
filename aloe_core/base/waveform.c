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
#include "waveform.h"
#include "mapping.h"
#include "str.h"

/**
 * Static temporal variable to do the mapping. After the mapping is done and modules assigned. Each module stores a pointer to the loaded node and keeps the processor index
 */
static mapping_t UNUSED(map);
static int UNUSED(transferring_node_id);

/**
 * Uses the platform object to load the waveform into the platform. The procedure is:
 * 1) Verify isParsed is true
 * 2) Use the private mapping instance calling mapping_map(&mapping,self)
 * 3) If the mapping is feasible,
 *  3.1) For each module set module[i].processor_idx=mapping.p_res[i] and module.node=man_platform_node_pidx(p_res[i]);
 * 4) For each node_id
 *  4.1) set mapped_nodes[i]=node_id
 *  4.2) set transferring_node_id=node_id
 *  4.3) serialize waveform to man_platform_node_id(node_id), command=LOAD
 * 6) Set the waveform status to LOADED
 */
int waveform_load(waveform_t *waveform) {
	aerror("Not yet implemented");
	return -1;
}

/**
 * updates the module's objects with the distributed modules in the platform.
 * 1) Verify Status.isLoaded()
 * 2) Iterate through the mapped_nodes_id array
 *   2.1) set transferring_node_id=node_id
 *   2.2) send command Cmd.GET to the man_platform_node_id(mapped_nodes_id[i]) with dest(waveform.Id,0,0)
 *   2.3) unserializeTo the received packet to the local waveform object
 * 4) set lastUpdateTS to current timeslot
 */
int waveform_update(waveform_t *obj) {
	aerror("Not yet implemented");
	return -1;
}

/**
 * Parses the waveform configuration file according to its format (specified in this class documentation). This method has to do:
 * 1) generate the (c,b) waveform model
 * 2) create a Module instance for each module and add to modules array
 * 3) create a Variable instance for each variable and add to modules[i].variables array
 * 4) set Status=STOP
 *
 *
 * *NOTE*: Use libconfig file format and library for parsing
 */
int waveform_parse(waveform_t* waveform) {
	aerror("Not yet implemented");
	return -1;
}

/**
 * - Serializes waveform attributes and call module.serialize() for all modules such that module->node.id=loading_node_id
 */
void waveform_serialize(waveform_t *src, packet_t *pkt) {
	aerror("Not yet implemented");
}

/**
 * Save waveform status. Check that each node has the same waveform status. First word is the module id. For each module, find module id and call unserializeTo(pkt,module).
 */
void waveform_unserializeTo(packet_t *pkt, waveform_t *dest) {
	aerror("Not yet implemented");
}
