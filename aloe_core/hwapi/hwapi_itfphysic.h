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

int hwapi_itfphysic_create(h_itf_t obj, string address);
int hwapi_itfphysic_remove(h_itf_t obj);
int hwapi_itfphysic_send(h_itf_t obj, void* buffer, int len);
int hwapi_itfphysic_recv(h_itf_t obj, void* buffer, int len);
int hwapi_itfphysic_set_callback(h_itf_t obj, void (*fnc)(void), int prio);
int hwapi_itfphysic_set_blocking(h_itf_t obj, int block);
int hwapi_itfphysic_get_blocking(h_itf_t obj);
h_pkt_t* hwapi_itfphysic_request_pkt(h_itf_t obj);
int hwapi_itfphysic_put_pkt(h_itf_t obj, h_pkt_t* ptr);
h_pkt_t* hwapi_itfphysic_get_pkt(h_itf_t obj);
int hwapi_itfphysic_release_pkt(h_itf_t obj, h_pkt_t* ptr);
int hwapi_itfphysic_set_delay(h_itf_t obj, int delay);
int hwapi_itfphysic_get_delay(h_itf_t obj);


#endif
