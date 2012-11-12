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

#ifndef MAN_DATAITF_H
#define MAN_DATAITF_H

#include "defs.h"
#include "str.h"

typedef struct {
/**
 * A physical data interface connects one node with another. This is the interface used by modules data exchange. Implements Serializable interface and just copies/saves attributes in both methods.
 */

	/**
	 * Unique physical interface id in the node
	 */
	int id;
	/**
	 * Interface address (see platform.conf in CmdPlatform class)
	 */
	strdef(address);
	int remoteItfId;
	int remoteNodeId;

} man_dataitf_t;

#endif
