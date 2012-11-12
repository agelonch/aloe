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

#ifndef MAN_PLATFORM_H
#define MAN_PLATFORM_H

#include "hwapi.h"
#include "man_node.h"
#include "packet.h"
#include "objects_max.h"
#include "manapi_types.h"

	/**
 * The CmdPlatform class manages the hardware parts of the platform. This config() function does not follow the platform-independence rules and is platform-dependant, because it reads the platform configuration file, which is particularized for a given platform (in this case, linux platform). The rest of the functions are platform-independent.
 * 
 * Sample platform configuration file format (libconfig format, see http://www.hyperrealm.com/libconfig/test.cfg.txt):
 * ===============================================================
 * 
 * platform:
 * {
 *  timeslot=100;                 // in microseconds. The time-slot is unique for the platform.
 *  
 *   other platform-wide options 
 *  mapping:
 *  {
 *    algorithm="tw";
 *    options="";
 *  };
 *    
 *  manager:
 *  {
 *    ip_address="192.168.1.1";   // IP address of the host where the manager will be executed
 *    clock_local=TRUE;           // If it runs in a node, it can obtain the timing using shared memory IPC. If set to false, it will obtain timing from synchronization.master
 *  };
 * 
 *  network:
 *  {
 *    ctrl ={port=7000; proto="tcp";}           // The manager connects to each node using this port
 *    probe={port=7001; proto="udp";}           // The manager connects to each node using this port
 *    sync ={port=7002; proto="udp";}           // Each node creates a listening socket on this range of ports
 *    data ={ports=[7003,7010]: proto="udp";}    These are the ports used for data interfaces. Each port is used to open a socket for every module-to-module link.
 *                                                In this example, each physical interface creates 7 ports, therefore up to logical links between modules can be used
 *                                                 in this physical interface 
 *  };       
 * 
 * };
 * 
 * 
 * node:
 * {                
 *  name="node1";
 *  ip_address=["192.168.1.1","224.0.0.1"];                   Comma-separated multiple ip's are supported.
 *                                                              Specifying a multi-cast address is useful for continuous synchronization with several nodes 
 *  cpu={mops=XXX; mbps=YYY; };
 *  scheduling={type=""; options=""; };
 * 
 *  dac=(
 *         {type="uhd"; address="usrp1"; options=""; },    // This indicates node1 has physical access to a AD/DA converter. supported types={"uhd","x5"}
 *                                                         // options string is passed as a parameter to DAC.create() function. See particular DAC documentation                                                       
 *         {  other dacs  }
 *      );
 * 
 *  other_options:
 *         {  rt_control="soft";                 RT-Fault if first module ("soft") or last module ("hard") still executing at begin of timeslot 
 *            rt_fault="kill";                   Kill waveform ("kill") or just print warning message ("print") 
 *            kernel_prio=60;                    Kernel timer thread priority (default=90) 
 *            proc_prio=59;                      processor threads priority   (default=89) 
 *        
 *             The following three optional values control the amount of memory that is preallocated at boot 
 *            max_waveforms=20;                  (optional) maximum number of waveform objects that can be used. Default=20 
 *            max_modules_x_waveform=20;         (optional) maximum number of module objects per waveform that can be used. Default=20 
 *            max_variables_x_module=20;         (optional) maximum number of variable objects per module that can be used. Default=20 
 *            max_modules_x_node=20;             (optional) maximum number of module objects per node that can be launched. Default=20 
 *            max_queues_x_node=20;              (optional) maximum number of queues per node that are created. Default=20        
 *         };        
 * };
 * 
 * 
 * node:
 * {                      // node2 has IP address 192.168.1.2
 *  name="node2";
 *  ip_address=["192.168.1.2"];
 *  cpu={mops=XXX; mbps=YYY; };
 *  scheduling={type=""; options=""; };
 *  
 *  other_options { rt_control="hard"; rt_fault="print"};
 * }
 * 
 * synchronization:
 * {
 *         master="node1";
 *         source="timer";          Options: "timer" for local timer; "dac://dac_name" to get clock from dac interrupts (master node needs access to the dac) 
 * 
 *         continuous_mode=TRUE;   Sends one packet at the beginning of the timeslot, which drives the slave timeslot clock. 
 *                                 If FALSE, slaves request synchronization packet every "period" timeslots 
 *         period=100;             Synchronization period. Ignored if continuous_mode=FALSE 
 *         multicast=TRUE;         If platform.manager.sync.proto="udp" and continuous_mode=TRUE, using multicast has better performance. 
 * };
 * 
 * data_interfaces= (
 *         {src="node1"; dst="node2"; bw=10.0; duplex="half";         node1 has an output interface to node2. bw is the interface bandwidth in MBPS     
 *                                                                   "nodeName:IPindex" optionally indicates the IP index in ip_address (default=0)
 *         { more interfaces here }
 *         );
 * 
 * 
 * ===============================================================
 */

typedef struct {


	man_node_t nodes[MAX(nodes)];
	man_platform_model_t model;
	packet_t packet;
	int last_update_ts;
	/**
	 * Time-slot duration, in microseconds
	 */
	int ts_length_us;
	h_phyitf_t *sync_itf;
	int sync_local;

} man_platform_t;

void man_platform_set_context(man_platform_t *context);
man_platform_t *man_platform_get_context();

int man_platform_config(string config_file);
int man_platform_sync();
int man_platform_send_cmd(packet_dest_t dest, packet_command_t cmd);
man_node_t* man_platform_node_id(int node_id);
man_node_t* man_platform_node_pidx(int processor_idx);
int man_platform_process_probe_pkt(void* data, int size);

#endif
