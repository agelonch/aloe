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

#ifndef HWAPI_ITFQUEUE_H
#define HWAPI_ITFQUEUE_H

#include "str.h"
#include "hwapi_itf.h"
#include "hwapi.h"
#include "queue.h"


typedef struct {
	hwapi_itf_t parent;

	int max_msg;
	int max_msg_sz;

	queue_t queue;
}hwapi_itfqueue_t;

int hwapi_itfqueue_init(hwapi_itfqueue_t *obj);
int hwapi_itfqueue_create(h_itf_t obj, string address);
int hwapi_itfqueue_remove(h_itf_t obj);
int hwapi_itfqueue_send(h_itf_t obj, void* buffer, int len);
int hwapi_itfqueue_recv(h_itf_t obj, void* buffer, int len);
int hwapi_itfqueue_set_callback(h_itf_t obj, void (*fnc)(void), int prio);
int hwapi_itfqueue_set_blocking(h_itf_t obj, int block);
int hwapi_itfqueue_get_blocking(h_itf_t obj);
h_pkt_t* hwapi_itfqueue_request_pkt(h_itf_t obj);
int hwapi_itfqueue_put_pkt(h_itf_t obj, h_pkt_t* ptr);
h_pkt_t* hwapi_itfqueue_get_pkt(h_itf_t obj);
int hwapi_itfqueue_release_pkt(h_itf_t obj, h_pkt_t* ptr);
int hwapi_itfqueue_set_delay(h_itf_t obj, int delay);
int hwapi_itfqueue_get_delay(h_itf_t obj);
#endif
