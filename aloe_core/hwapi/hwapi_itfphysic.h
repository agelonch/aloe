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

#ifndef HWAPI_ITFPHYSIC_H
#define HWAPI_ITFPHYSIC_H

#include "str.h"
#include "hwapi_itf.h"
#include "hwapi.h"


/**
 * In linux, physical interfaces are TCP/IP Ethernet interfaces. They inherit from the Interface abstract class. Supported protocols are TCP and UDP. Modes can be IN, which creates an input socket and accepts the connection in TCP; or OUT, which creates the output socket.
 */

typedef struct {
	hwapi_itf_t parent;
	int sock_fd;
	int conn_fd;
	int is_tcp;
	int connected;
	int id;
}hwapi_itfphysic_t;



#endif
