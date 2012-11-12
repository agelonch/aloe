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

#include <stddef.h>
#include "queue.h"

void queue_init(queue_t *q) {
	q->count = 0;
	q->front = 0;
	q->rear = -1;
}

int queue_is_empty(queue_t *q) {
	return (q->count == 0);
}

int queue_is_full(queue_t *q) {
	return (q->count == MAX_QUEUE_SZ);
}

int queue_put(queue_t *q, void *value) {
	if (queue_is_full(q)) {
		return -1;
	}
	q->count++;
	q->rear++;
	if (q->rear == MAX_QUEUE_SZ) {
		q->rear = 0;
	}
	q->items[q->rear] = value;
	return 0;
}

void *queue_get(queue_t *q) {
	void *x;
	if (queue_is_empty(q)) {
		return NULL;
	}
	q->count--;
	x = q->items[q->count];
	q->front++;
	if (q->front == MAX_QUEUE_SZ) {
		q->front = 0;
	}
	return x;
}
