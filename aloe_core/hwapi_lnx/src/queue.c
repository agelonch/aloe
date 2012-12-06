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
#include <pthread.h>
#include <stddef.h>
#include "queue.h"

void queue_init(queue_t *q) {
	q->count = 0;
	q->front = 0;
	q->rear = -1;
	if (pthread_mutex_init(&q->mutex, NULL)) {
		printf("error initiating mutex\n");
	}
}

int queue_is_empty(queue_t *q) {
	return (q->count == 0);
}

int queue_is_full(queue_t *q) {
	return (q->count == MAX_QUEUE_SZ);
}

int queue_put(queue_t *q, void *value) {
	pthread_mutex_lock(&q->mutex);
	if (queue_is_full(q)) {
		pthread_mutex_unlock(&q->mutex);
		return -1;
	}
	q->count++;
	q->rear++;
	if (q->rear == MAX_QUEUE_SZ) {
		q->rear = 0;
	}
	q->items[q->rear] = value;
	pthread_mutex_unlock(&q->mutex);
	return 0;
}

void *queue_get(queue_t *q, int tstamp) {
	int *x;
	pthread_mutex_lock(&q->mutex);
	if (queue_is_empty(q)) {
		pthread_mutex_unlock(&q->mutex);
		return NULL;
	}
	x = q->items[q->count-1];
	if (tstamp) {
		if (tstamp < *x ) {
			printf("massa dora. pkt %d, tstamp=%d\n",*x,tstamp);
			pthread_mutex_unlock(&q->mutex);
			return NULL;
		}
	}

	q->count--;
	x = q->items[q->count];
	q->front++;
	if (q->front == MAX_QUEUE_SZ) {
		q->front = 0;
	}
	pthread_mutex_unlock(&q->mutex);
	return x;
}
