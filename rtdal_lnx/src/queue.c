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

#include <string.h>
#include <stdlib.h>
#include "queue.h"
#include "rtdal.h"
#include "defs.h"

void queue_init(queue_t *q, int pkt_sz, int nof_pkts) {
	int i;

	q->read = 0;
	q->write = 0;
	q->size = nof_pkts;
	q->data = malloc(nof_pkts*pkt_sz);
	q->packets = malloc(nof_pkts*sizeof(h_pkt_t));
	for (i=0;i<nof_pkts;i++) {
		q->packets[i].data = &q->data[i*pkt_sz];
		q->packets[i].valid = 0;
	}
}

void queue_free(queue_t *q) {
	if (q->data) {
		free(q->data);
		q->data = NULL;
	}
	if (q->packets) {
		free(q->packets);
		q->packets = NULL;
	}
}

inline int queue_is_empty(queue_t *q) {
	return (q->packets[q->read].valid == 0);
}

inline int queue_is_full(queue_t *q) {
	return (q->packets[q->write].valid != 0);
}

h_pkt_t* queue_request(queue_t *q) {
	qdebug("write=%d, tstamp=%d\n",q->write,q->packets[q->write].tstamp);
	if (queue_is_full(q)) {
		return NULL;
	}
	return &q->packets[q->write];
}

int queue_push(queue_t *q) {
	qdebug("write=%d, tstamp=%d\n",q->write,q->packets[q->write].tstamp);
	q->packets[q->write].valid = 1;
	q->write += (q->write+1 >= q->size) ? (1-q->size) : 1;
	return 0;
}

h_pkt_t *queue_pop(queue_t *q, int tstamp) {


	if (queue_is_empty(q)) {
		qdebug("[empty] read=%d, tstamp=%d\n",q->read,q->packets[q->read].tstamp);
		return NULL;
	}
	if (tstamp) {
		if (q->packets[q->read].tstamp > tstamp) {
			qdebug("[delay] read=%d, tstamp=%d\n",q->read,q->packets[q->read].tstamp);
			return NULL;
		}
		if (q->packets[q->read].tstamp < tstamp) {
			qdebug("[late] read=%d tstamp=%d ara=%d\n",q->read,q->packets[q->read].tstamp,tstamp);
		}
	}
	qdebug("[ok] read=%d, tstamp=%d (now=%d)\n",q->read,q->packets[q->read].tstamp,tstamp);
	return &q->packets[q->read];
}

int queue_release(queue_t *q) {
	qdebug("read=%d, tstamp=%d\n",q->read,q->packets[q->read].tstamp);
	q->packets[q->read].valid = 0;
	q->read += (q->read+1 >= q->size) ? (1-q->size) : 1;
	return 0;
}
