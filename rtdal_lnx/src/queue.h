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

#ifndef QUEUE_H_
#define QUEUE_H_

#include "rtdal_types.h"

typedef struct {
	int read;
	int write;
	int size;
	int id;
	h_pkt_t *packets;
	char *data;
}queue_t;


void queue_init(queue_t *q, int pkt_sz, int nof_pkts);
void queue_free(queue_t *q);
int queue_is_empty(queue_t *q);
int queue_is_full(queue_t *q);
h_pkt_t* queue_request(queue_t *q);
int queue_push(queue_t *q);
h_pkt_t *queue_pop(queue_t *q, int tstamp);
int queue_release(queue_t *q);
#endif
