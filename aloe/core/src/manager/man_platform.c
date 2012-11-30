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

#include <stddef.h>

#include "defs.h"
#include "str.h"
#include "man_platform.h"

static man_platform_t *platform = NULL;


man_platform_t *man_platform_get_context() {
	return platform;
}

/**
 * For each node, send a HWINFO command. Save the contents of the received packet into
 * node.hwinfo structure
 */
int man_platform_update() {
	aerror("Not yet implemented");
	return -1;
}

/**
 * 1) For each node, call node[i].ctrlItf.connect() and node[i].probeItf.connect()
 * 3) If syncLocal==false, syncItf.connect()
 * 4) For each node, send Cmd.CONNECT command
 */
int man_platform_connect() {
	aerror("Not yet implemented");
	return -1;
}

/**
 * 1) For each node, send Cmd.DISCONNECT command
 * 2) For each node, call node.ctrlItf.close(), node.probeItf.close()
 * 3) call syncItf.close()
 */
int man_platform_disconnect() {
	aerror("Not yet implemented");
	return -1;
}

int man_platform_tslot_length() {
	aerror("Not yet implemented");
	return -1;
}

man_platform_model_t* man_platform_get_model() {
	aerror("Not yet implemented");
	return NULL;
}

/**
 * Initializes the HWAPI and reads and parses the platform configuration file with
 * path "configFile".
 * It must be called before calling any other function.
 *
 * 1) Parse "configFile"
 * 2) Call hwapi_create_manager_itfs(configFile)
 * 3) Configure clock:
 *   3.1) If clockLocal==TRUE, call hwapi_attach_local_time() and set sync_local=TRUE
 *   3.2) Otherwise, set sync_itf=hwapi_physicitf_get("sync"); and set sync_local=FALSE
 * 4) For each node
 *   4.1) Configure node, processor and interfaces instances
 *   4.2) set node[i].ctrl_itf=hwapi_itfphysic_get("ctrl_nodename") where nodename is the node name
 *   4.3) set node[i].probe_itf=hwapi_itfphysic_get("probe_nodename") where nodename is the node name 
 *   4.4) Add asyncError to all probe interfaces listeners. Node.probeListener.addCallback(process_probe_pkt)
 *
 * 5) Create C, B, I platform model from the nodes and data interfaces specified in
 * the configuration file
 *
 * The function must check that the configuration file parameters are correct, issuing
 * warnings when necessary
 */

int man_platform_config(man_platform_t *_platform, string config_file) {
	mdebug("platform=0x%x, config_file=%s\n",platform,config_file);

	hwapi_machine_t machine;

	if (!_platform) {
		return -1;
	}
	hwapi_machine(&machine);

	platform = _platform;
	platform->nof_nodes = 1;
	platform->nof_processors = machine.nof_cores;
	for (int i=0;i<platform->nof_processors;i++) {
		platform->nodes[0].processors[i].node = &platform->nodes[0];
		platform->processors[i] = &platform->nodes[0].processors[i];
		platform->nodes[0].processors[i].idx_in_node = i;
	}
	platform->nodes[0].id = 1;
	platform->nodes[0].platform = platform;

	return 0;
}

/**
 * Synchronizes with the master clock reference (using shared memory if master is in the same computer
 * or sync interfaces otherwise.
 * @return
 */
int man_platform_sync() {
	aerror("Not yet implemented");
	return -1;
}

/**
 * Sends a command to a destination. Packet must be filled before.
 */
int man_platform_send_cmd(packet_dest_t dest, packet_command_t cmd) {
	aerror("Not yet implemented");
	return -1;
}


/**
 * Processes asynchronous execution error messages from nodes. The format of the asyncError packet is as follows:
 *
 * 1st 32-bit word: Waveform id
 * 2st 32-bit word: Module id
 * 3st 32-bit word: Error code
 * Next: aerrorMsg string
 *
 * For any error message!=OK, set STOP status to the waveform that caused it and print message to SDTDOUT.
 */
int man_platform_process_probe_pkt(void* data, int size) {
	aerror("Not yet implemented");
	return -1;
}
