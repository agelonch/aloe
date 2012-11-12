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

	queue_t q_tx;
	queue_t q_pkts;

	h_pkt_t *packets;
	void *data;
}hwapi_itfqueue_t;

int hwapi_itfqueue_init(hwapi_itfqueue_t *obj);

#endif
