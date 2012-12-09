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

#ifndef rtdal_TASK_H_
#define rtdal_TASK_H_

#define TASK_DEFAULT_PRIORITY 10
#define TASK_DEFAULT_CPUID -1

#include <pthread.h>
#include "rtdal.h"

enum rtdal_task_type {
	DETACHABLE, JOINABLE
};

int rtdal_task_new_thread(pthread_t *thread, void *(*fnc)(void*), void *arg, \
		enum rtdal_task_type type, int prio, int cpu, int unblock_signals);
int rtdal_task_new_prio(r_task_t *task, void *(*fnc)(void*), void* arg, int prio, int cpu);

#endif
