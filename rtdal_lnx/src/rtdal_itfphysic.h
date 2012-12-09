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

#ifndef rtdal_ITFPHYSIC_H
#define rtdal_ITFPHYSIC_H

#include "str.h"
#include "rtdal_itf.h"
#include "rtdal.h"


/**
 * In linux, physical interfaces are TCP/IP Ethernet interfaces. They inherit from the Interface abstract class. Supported protocols are TCP and UDP. Modes can be IN, which creates an input socket and accepts the connection in TCP; or OUT, which creates the output socket.
 */

typedef struct {
	rtdal_itf_t parent;
	int sock_fd;
	int conn_fd;
	int is_tcp;
	int connected;
	int id;
}rtdal_itfphysic_t;

int rtdal_itfphysic_create(h_itf_t obj, string address);
int rtdal_itfphysic_remove(h_itf_t obj);
int rtdal_itfphysic_send(h_itf_t obj, void* buffer, int len);
int rtdal_itfphysic_recv(h_itf_t obj, void* buffer, int len);
int rtdal_itfphysic_set_callback(h_itf_t obj, void (*fnc)(void), int prio);
int rtdal_itfphysic_set_blocking(h_itf_t obj, int block);
int rtdal_itfphysic_get_blocking(h_itf_t obj);
h_pkt_t* rtdal_itfphysic_request_pkt(h_itf_t obj);
int rtdal_itfphysic_put_pkt(h_itf_t obj, h_pkt_t* ptr);
h_pkt_t* rtdal_itfphysic_get_pkt(h_itf_t obj);
int rtdal_itfphysic_release_pkt(h_itf_t obj, h_pkt_t* ptr);
int rtdal_itfphysic_set_delay(h_itf_t obj, int delay);
int rtdal_itfphysic_get_delay(h_itf_t obj);


#endif
