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
#include <strings.h>

#include "str.h"
#include "defs.h"
#include "objects_max.h"
#include "waveform.h"
#include "packet.h"
#include "mempool.h"

#include "manapi.h"
#include "manapi_context.h"
#include "manapi_error.h"
#include "man_platform.h"

manapi_context_t manapi;
man_platform_t *platform;
packet_t *man_packet;

#define MAPPING_MAX_MODULES 100

/**
 * Creates the CmdPlatform object from the platformConfPath configuration file.
 * call read_waveforms
 */
int manapi_initialize(string platform_conf, int max_waveforms) {

	memset(&manapi, 0, sizeof(manapi_context_t));

	man_packet = &manapi.platform.packet;
	if (packet_init(&manapi.platform.packet, 5*1024)) {
		aerror("initializing packet\n");
		return -1;
	}

	manapi_error_set_context(&manapi.error);

	if (man_platform_config(&manapi.platform, platform_conf)) {
		return -1;
	}
	manapi.waveforms = (waveform_t*) pool_alloc(max_waveforms,sizeof(waveform_t));
	if (!manapi.waveforms) {
		return -1;
	}
	manapi.max_waveforms = max_waveforms;
	manapi.initialized = 1;
	return 0;
}

/**
 * returns the CmdWaveform object associated to the waveform "name"
 */
waveform_t* manapi_waveform_get(string name) {
	int i;
	if (!manapi.initialized) return NULL;

	i=0;
	while(i<manapi.max_waveforms && strncmp(manapi.waveforms[i].name,name,STR_LEN))
		i++;
	if (i == manapi.max_waveforms) {
		return NULL;
	}
	return &manapi.waveforms[i];
}

/**
 * Reads the repository directory and creates and initializes the waveform instances objects,
 * one for each waveform model file in the directory (.app extension).
 */
int manapi_read_waveforms(string repository) {
	string filename = repository; // this is temporal

	strcpy(manapi.waveforms[0].model_file,filename);

	char *x = index(filename,'.');
	if (!x) {
		return -1;
	}
	*x = '\0';
	strcpy(manapi.waveforms[0].name,filename);
	return -1;
}

/**
 * rsh runcf on all nodes
 */
int manapi_start_nodes() {
	aerror("Not yet implemented");
	return -1;
}

/**
 * rsh killcf on all nodes
 */
int manapi_stop_nodes() {
	aerror("Not yet implemented");
	return -1;
}
