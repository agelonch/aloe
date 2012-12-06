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
#include "packet.h"
#include "nod_dispatcher.h"
#include "nod_anode.h"

extern nod_anode_t anode;


/** \brief Loads a waveform serialized in pkt. Finds an empty (status=STOP) space in
 * anode.loaded_waveforms and calls the nod_waveform_unserializeTo() function. Then calls
 * nod_waveform_load() which loads the binaries to the hwapi pipelines and then calls
 * nod_waveform_run() which sets all modules as runnable.
 * \returns 0 on successs, -1 on error
 */
static int nod_dispatcher_load(packet_t *pkt) {
	/* @TODO: Error messages */
	int i;
	aassert(pkt);

	for (i=0;i<anode.max_waveforms;i++) {
		if (anode.loaded_waveforms[i].status.cur_status == STOP) {
			break;
		}
	}
	if (i == anode.max_waveforms) {
		aerror("Maximum number of waveforms per node reached\n");
		return -1;
	}
	ndebug("waveform_idx=%d\n",i);
	if (nod_waveform_unserializeTo(pkt,&anode.loaded_waveforms[i])) {
		return -1;
	}
	if (nod_waveform_load(&anode.loaded_waveforms[i])) {
		ndebug("error loading waveform_idx=%d. Removing\n",i);
		nod_waveform_remove(&anode.loaded_waveforms[i]);
		return -1;
	}

	return 0;
}
/**
 * 1) Use processing_packet.getDest() to get the destination object
 * 2) Find waveformId
 * 3) If moduleId=0, unserializeTo(waveform.status) and return OK
 * 4) Find moduleId
 * 5) If variableId=0 do nothing (nothing implemented) return ERR
 * 6) Find variableId, unserializeTo(waveform.module.variable) and return OK
 *
 * If at any stage, the Id is not found, return ERR
 */
static int nod_dispatcher_set(packet_t *pkt) {
	packet_dest_t *dest = packet_get_dest(pkt);
	int wi,mi,vi;
	ndebug("dest=(%d,%d,%d)\n",dest->waveform_id,dest->module_id,dest->variable_id);
	if (!dest->waveform_id) {
		return -1;
	}
	for (wi=0;wi<anode.max_waveforms;wi++) {
		if (anode.loaded_waveforms[wi].id == dest->waveform_id)
			break;
	}
	if (wi == anode.max_waveforms) {
		return -1;
	}
	ndebug("found wi=%d\n",wi);
	if (!dest->module_id && !dest->variable_id) {
		if (nod_waveform_unserializeTo(pkt, &anode.loaded_waveforms[wi])) {
			return -1;
		}
		return 0;
	}
	if (!dest->module_id) {
		return -1;
	}
	for (mi=0;mi<anode.loaded_waveforms[mi].nof_modules;mi++) {
		if (anode.loaded_waveforms[wi].modules[mi].parent.id == dest->module_id)
			break;
	}
	if (mi == anode.loaded_waveforms[wi].nof_modules) {
		return -1;
	}
	ndebug("found mi=%d\n",mi);
	if (!dest->variable_id) {
		aerror("Not implemented\n");
		return -1;
	}
	for (vi=0;vi<anode.loaded_waveforms[mi].modules[mi].parent.nof_variables;vi++) {
		if (anode.loaded_waveforms[wi].modules[mi].parent.variables[vi].id ==
				dest->variable_id)
			break;
	}
	if (vi == anode.loaded_waveforms[mi].modules[mi].parent.nof_variables) {
		return -1;
	}
	if (variable_unserializeTo(pkt, &anode.loaded_waveforms[wi].modules[mi].parent.variables[vi],
			CP_VALUE,anode.loaded_waveforms[mi].nof_modes)) {
		return -1;
	}
	return 0;
}

/**
 * @TODO
 */
static int nod_dispatcher_get(packet_t *pkt) {
	packet_dest_t *dest = packet_get_dest(pkt);
	int wi,mi,vi;
	ndebug("dest=(%d,%d,%d)\n",dest->waveform_id,dest->module_id,dest->variable_id);
	if (!dest->waveform_id) {
		return -1;
	}
	for (wi=0;wi<anode.max_waveforms;wi++) {
		if (anode.loaded_waveforms[wi].id == dest->waveform_id)
			break;
	}
	if (wi == anode.max_waveforms) {
		return -1;
	}
	ndebug("found wi=%d\n",wi);
	if (!dest->module_id && !dest->variable_id) {
		if (nod_waveform_serialize(&anode.loaded_waveforms[wi],pkt, 0, NONE)) {
			return -1;
		}
		return 0;
	}
	if (!dest->module_id) {
		return -1;
	}
	for (mi=0;mi<anode.loaded_waveforms[mi].nof_modules;mi++) {
		if (anode.loaded_waveforms[wi].modules[mi].parent.id == dest->module_id)
			break;
	}
	if (mi == anode.loaded_waveforms[wi].nof_modules) {
		return -1;
	}
	ndebug("found mi=%d\n",mi);
	if (!dest->variable_id) {
		aerror("Not implemented\n");
		return -1;
	}
	for (vi=0;vi<anode.loaded_waveforms[mi].modules[mi].parent.nof_variables;vi++) {
		if (anode.loaded_waveforms[wi].modules[mi].parent.variables[vi].id ==
				dest->variable_id)
			break;
	}
	if (vi == anode.loaded_waveforms[mi].modules[mi].parent.nof_variables) {
		return -1;
	}
	if (variable_serialize(&anode.loaded_waveforms[wi].modules[mi].parent.variables[vi], pkt,
			CP_VALUE,anode.loaded_waveforms[mi].nof_modes)) {
		return -1;
	}
	return 0;
}

/**
 * 1) Connect all output data interfaces
 */
static int nod_dispatcher_connect_node() {
	aerror("Not yet implemented");
	return -1;
}

/**
 * 1) Disconnect all output interfaces
 */
static int nod_dispatcher_disconnect_node() {
	aerror("Not yet implemented");
	return -1;
}

/**
 * Copies the contents of the hwapi.machine structure to the packet
 */
int nod_dispatcher_hwinfo(packet_t *pkt) {
	aerror("Not yet implemented");
	return -1;
}

/** \brief Processes a packet. Calls one of the processing functions depending on the command
 */
int nod_anode_dispatch(packet_t *pkt) {
	int n;
	ndebug("pkt=0x%x, cmd=%d\n",pkt,packet_get_cmd(pkt));
	switch(packet_get_cmd(pkt)) {
	case CMD_LOAD:
		n = nod_dispatcher_load(pkt);
		packet_clear(pkt);
		break;
	case CMD_SET:
		n =  nod_dispatcher_set(pkt);
		packet_clear(pkt);
		break;
	case CMD_GET:
		n =  nod_dispatcher_get(pkt);
		break;
	case CMD_CONNECT:
		n =  nod_dispatcher_connect_node(pkt);
		break;
	case CMD_DISCONNECT:
		n =  nod_dispatcher_disconnect_node(pkt);
		break;
	case CMD_HWINFO:
		n =  nod_dispatcher_hwinfo(pkt);
		break;
	default:
		aerror_msg("Unknown command %d\n", (int) packet_get_cmd(pkt));
		n =  -1;
		break;
	}
	return n;
}
