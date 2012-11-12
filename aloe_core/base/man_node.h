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

#ifndef MAN_NODE_H
#define MAN_NODE_H

#include "man_dataitf.h"
#include "man_processor.h"
#include "man_probelistener.h"
#include "objects_max.h"
#include "hwapi.h"

/**
 * A node is a collection of processors sharing memory. It may have data interfaces to other nodes. It implements Serializable interface and just copies all attributes
 */

typedef struct {

	void *platform;
	man_processor_t man_processors[MAX(processors)];
	man_dataitf_t man_dataitfs[MAX(hwapi_itfphysic)];
	man_probelistener_t probe_listener;
	h_phyitf_t* ctr_itf;
	h_phyitf_t* probe_itf;

	/**
	 * Name of the node, as specified in platform.conf in CmdPlatform class
	 */
	strdef(name);
	int id;

} man_node_t;

#endif
