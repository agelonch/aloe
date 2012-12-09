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

#ifndef rtdal_ITFspscq_H
#define rtdal_ITFspscq_H

#include "str.h"
#include "rtdal_itf.h"
#include "rtdal.h"


typedef struct {
	int valid;
	int tstamp;
	int len;
	void *data;
}r_pkt_t;

typedef struct {
	rtdal_itf_t parent;

	int max_msg;
	int max_msg_sz;
	int read;
	int write;
	r_pkt_t *packets;
	char *data;
}rtdal_itfspscq_t;

int rtdal_itfspscq_init(rtdal_itfspscq_t *obj);
int rtdal_itfspscq_create(r_itf_t obj, string address);
int rtdal_itfspscq_remove(r_itf_t obj);
int rtdal_itfspscq_request(r_itf_t obj, void **ptr);
int rtdal_itfspscq_push(r_itf_t obj, int len);
int rtdal_itfspscq_pop(r_itf_t obj, void **ptr, int *len);
int rtdal_itfspscq_release(r_itf_t obj);
int rtdal_itfspscq_send(r_itf_t obj, void* buffer, int len);
int rtdal_itfspscq_recv(r_itf_t obj, void* buffer, int len);
int rtdal_itfspscq_set_callback(r_itf_t obj, void (*fnc)(void), int prio);
int rtdal_itfspscq_set_blocking(r_itf_t obj, int block);
int rtdal_itfspscq_get_blocking(r_itf_t obj);
int rtdal_itfspscq_set_delay(r_itf_t obj, int delay);
int rtdal_itfspscq_get_delay(r_itf_t obj);
#endif
