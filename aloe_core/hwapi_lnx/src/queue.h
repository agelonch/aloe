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

#define MAX_QUEUE_SZ	64

#include <pthread.h>

typedef struct {
	int count;
	int front;
	int rear;
	pthread_mutex_t mutex;
	void *items[MAX_QUEUE_SZ];
}queue_t;


void queue_init(queue_t *q);
int queue_is_empty(queue_t *q);
int queue_is_full(queue_t *q);
int queue_put(queue_t *q, void *value);
void *queue_get(queue_t *q, int tstamp);
#endif
