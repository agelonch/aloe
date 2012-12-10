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

#include "oesr_man.h"
#include "oesr_man_context.h"
#include "oesr_man_error.h"
#include "man_platform.h"

oesr_man_context_t oesr_man;
man_platform_t *platform;
packet_t *man_packet;

#define MAPPING_MAX_MODULES 100

/**
 * Creates the CmdPlatform object from the platformConfPath configuration file.
 * call read_waveforms
 */
int oesr_man_initialize(string platform_conf, int max_waveforms) {

	memset(&oesr_man, 0, sizeof(oesr_man_context_t));

	man_packet = &oesr_man.platform.packet;
	if (packet_init(&oesr_man.platform.packet, 5*1024)) {
		aerror("initializing packet\n");
		return -1;
	}

	oesr_man_error_set_context(&oesr_man.error);

	if (man_platform_config(&oesr_man.platform, platform_conf)) {
		return -1;
	}
	oesr_man.waveforms = (waveform_t*) pool_alloc(max_waveforms,sizeof(waveform_t));
	if (!oesr_man.waveforms) {
		return -1;
	}
	oesr_man.max_waveforms = max_waveforms;
	oesr_man.initialized = 1;
	return 0;
}

/**
 * returns the CmdWaveform object associated to the waveform "name"
 */
waveform_t* oesr_man_waveform_get(string name) {
	int i;
	if (!oesr_man.initialized) return NULL;

	i=0;
	while(i<oesr_man.max_waveforms && strncmp(oesr_man.waveforms[i].name,name,STR_LEN))
		i++;
	if (i == oesr_man.max_waveforms) {
		return NULL;
	}
	return &oesr_man.waveforms[i];
}

/**
 * Reads the repository directory and creates and initializes the waveform instances objects,
 * one for each waveform model file in the directory (.app extension).
 */
int oesr_man_read_waveforms(string repository) {
	string filename = repository; // this is temporal

	strcpy(oesr_man.waveforms[0].model_file,filename);

	char *x = index(filename,'.');
	if (!x) {
		return -1;
	}
	*x = '\0';
	strcpy(oesr_man.waveforms[0].name,filename);
	return -1;
}

/**
 * rsh runcf on all nodes
 */
int oesr_man_start_nodes() {
	aerror("Not yet implemented");
	return -1;
}

/**
 * rsh killcf on all nodes
 */
int oesr_man_stop_nodes() {
	aerror("Not yet implemented");
	return -1;
}
