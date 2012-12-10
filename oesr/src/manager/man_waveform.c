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

#include "str.h"
#include "defs.h"
#include "packet.h"
#include "waveform.h"
#include "man_mapping.h"
#include "serializable.h"
#include "man_platform.h"
#include "oesr_man.h"


static mapping_t map;

/** \brief Serializes a waveform and sends the packet to each of node it has modules allocated
 * to. Waits for the ACK from each node and returns 0 on success, -1 on error.
 */
static int waveform_send(waveform_t *waveform, packet_command_t cmd) {
	mdebug("waveform_id=%d\n",waveform->id);
	packet_dest_t dest;
	man_platform_t *platform = man_platform_get_context();
	int i;
	for (i=0;i<MAX(nodes);i++) {
		if (waveform->modules_x_node[i]) {
			mdebug("sending %d modules to node_id=%d\n",map.modules_x_node[i],
					platform->nodes[i].id);
			packet_clear(&platform->packet);
			if (waveform_serialize(waveform,&platform->packet,platform->nodes[i].id,
					CP_INIT)) {
				return -1;
			}
			packet_set_cmd(&platform->packet, cmd);
			dest.module_id = 0;
			dest.node = &platform->nodes[i];
			dest.variable_id = 0;
			dest.waveform_id = waveform->id;
			if (packet_sendto(&platform->packet,&dest)) {
				return -1;
			}
			/** @TODO: In multi-processor platform, this should be something else
			 *
			 */
			if (packet_get_ack(&platform->packet)) {
				return -1;
			}
		}
	}
	return 0;
}

/** \brief Loads waveform to the platform. For each node that has non-zero allocated modules,
 * serializes the waveform setting loading_node_id parameter to node.id. The parameter CP_INIT
 * is also used for copy_data, which indicates that the initialization value for each variables
 * will be transfered to each node.
 * \returns 0 on success or -1 on error. The error message is saved using the oesr_man_error class.
 */
int waveform_load(waveform_t *waveform) {
	/**@TODO: Use the oesr_man_error class for error messages */
	mdebug("waveform_id=%d, waveform_name=%s\n",waveform->id, waveform->name);

	aassert(waveform);
	/* @FIXME: temporary avoid checking this since there is no node feedback */
	/*if (waveform->status.cur_status != PARSED) {
		return -1;
	}*/
	waveform->status.cur_status = PARSED;

	if (mapping_map(&map, waveform)) {
		return -1;
	}
	/* save map.modules_x_node vector before calling waveform_send */
	memcpy(waveform->modules_x_node,
			map.modules_x_node,MAX(nodes)*sizeof(int));

	if (waveform_send(waveform, CMD_LOAD)) {
		return -1;
	}
	waveform->status.cur_status = LOADED;
	return 0;
}

/**
 @TODO update from each node
 */
int waveform_update(waveform_t *waveform) {
	packet_dest_t dest;
	man_platform_t *platform = man_platform_get_context();
	packet_set_cmd(&platform->packet, CMD_GET);
	dest.module_id = 0;
	dest.node = &platform->nodes[0];
	dest.variable_id = 0;
	dest.waveform_id = waveform->id;
	packet_clear(&platform->packet);
	if (packet_sendto(&platform->packet,&dest)) {
		return -1;
	}
	if (waveform_unserializeTo(&platform->packet, waveform,NONE)) {
		return -1;
	}
}


/** \brief Returns 1 if the waveform is allowed to transmit from the current status to the
 * new_status passed as second parameter, zero otherwise
 */
static int waveform_status_is_valid(waveform_t *waveform, waveform_status_t *new_status) {
	mdebug("waveform_id=%d, status=%d, new_status=%d\n",waveform->id, waveform->status.cur_status,
			new_status->cur_status);

	switch(new_status->cur_status) {
	case PARSED:
	case LOADED:
		return 0;
	case INIT:
		if (waveform->status.cur_status == LOADED)
			return 1;
		else
			return 0;
	case RUN:
		if (waveform->status.cur_status == PAUSE || waveform->status.cur_status == INIT
				|| waveform->status.cur_status == STEP)
			return 1;
		else
			return 0;
	case STEP:
		if (waveform->status.cur_status == RUN || waveform->status.cur_status == INIT
				|| waveform->status.cur_status == PAUSE || waveform->status.cur_status == STEP)
			return 1;
		else
			return 0;
	case PAUSE:
		if (waveform->status.cur_status == RUN || waveform->status.cur_status == STEP)
			return 1;
		else
			return 0;
	case STOP:
		return 1;

	}
	return 0;
}
/**
 * \brief Modifies the status of a waveform. The structure waveform_status_t is defined as
 * typedef struct {
 * 	waveform_status_enum cur_status;
 * 	int next_timeslot;
 * 	int dead_timeslot;
 * } waveform_status_t;
 *
 * cur_status indicates the new status to change. next_timeslot is desired time to change to the
 * new status and dead_timeslot is the status change deadline. An error will be returned if
 * this deadline is exceeded and the waveform did not successfully changed to the new status.
 */
int waveform_status_set(waveform_t *waveform, waveform_status_t *new_status) {
	aassert(waveform);
	aassert(new_status);
	mdebug("waveform_id=%d, new_status=%d, cur_status=%d\n", waveform->id, new_status->cur_status,
			waveform->status.cur_status);

	if (!waveform_status_is_valid(waveform,new_status)) {
		return -1;
	}

	memcpy(&waveform->status,new_status,sizeof(waveform_status_t));

	if (waveform_send(waveform, CMD_SET)) {
		return -1;
	}
	if (new_status->cur_status == STOP) {
		waveform->status.cur_status = PARSED;
	} else {
		waveform->status.cur_status = new_status->cur_status;
	}

	return 0;
}

/**
 * \brief returns true if the Status is LOADED, INIT, RUN or PAUSE
 */
int waveform_status_is_loaded(waveform_status_t *status) {
	mdebug("status=%d\n",status->cur_status);
	return (status->cur_status == LOADED || status->cur_status == INIT
			|| status->cur_status == PAUSE || status->cur_status == RUN);
}

/**
 * \brief returns true if the waveform is in status RUN or PAUSE
 */
int waveform_status_is_running(waveform_status_t *status) {
	mdebug("status=%d\n",status->cur_status);
	return (status->cur_status == RUN || status->cur_status == PAUSE);
}



/**
 * Gets the variable of the module running in an ALOE-node and saves the value to the buffer "value". Only min("size",variable.size) bytes will be updated.
 * 1) Verify module.waveform.Status=RUN
 * 2) Send Cmd.GET dest(variable.module.waveform.Id, variable.module.Id, variable.Id) to variable.module.node using variable.module.waveform.cmdPlatform
 * 3) wait for answer and serializeTo(variable) the packet
 * 4) update lastUpdateTS with current ts
 */
int variable_update(variable_t *obj) {
	aerror("Not yet implemented");
	return -1;
}

/**
 * Begins a continuous report of a variable value with a given period and window. Each timeslot, the module produces a new variable value, which is received by the manager who calls the function pointed by "fncPtr".
 * 1) Verify module.waveform.Status.isRunning()
 * 2) Use module.node.probeListener.addCallback(fncPtr, variable.Id) to setup a callback handler to the Probe physical interface.
 * 3) save window and period attributes. Save size to temporal variable. Set size to 0.
 * 4) Send Cmd.SET dest(variable.module.waveform.Id, variable.module.Id, variable.Id) to module.node using module.waveform.cmdPlatform and serialize(variable)
 * 5) Restore size from temporal variable.
 * 6) Waits for ACK. If OK quits otherwise removes the callback using module.node.probeListener.removeCallback(variable.Id)
 */
int variable_report_start(variable_t *var, void (*callback) (void), int period, int window) {
	aerror("Not yet implemented");
	return -1;
}

/**
 * Stops reporting a variable. Sets window=period=0 and serializes de variable using SET command. The variable then not generate any other packet. This function then removes the callback from module.node.probeListener
 */
int variable_report_stop(variable_t *var) {
	aerror("Not yet implemented");
	return -1;
}


/**
 * Updates the local object's attributes with the node's attributes. Retrieves also all the variables and execinfo
 * 1) Verify waveform.Status.isLoaded()
 * 2) Send Cmd.GET to dest(module.waveform.Id, module.Id,0) to module.node using module.waveform.cmdPlatform
 * 3) wait for answer and serializeTo(module)
 * 4) update lastUpdateTS with current ts
 */
int module_update(module_t *obj) {
	aerror("Not yet implemented");
	return -1;
}

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



