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

#ifndef TIMER_H_
#define TIMER_H_

#include <pthread.h>
#include <sys/time.h>
#include <time.h>

#define TIMER_FUTEX_GUARD_SEC 	2
#define KERNEL_OFFSET_USEC	30
enum timer_mode {
	NANOSLEEP, TIMERFD
};

typedef struct
{
	struct timespec next;
	int timer_fd;
	unsigned long long wakeups_missed;
	struct itimerspec itval;
	long int period;
	int stop;
	pthread_t *thread;
	int *wait_futex;
	void *arg;
	void (*period_function)(void*, struct timespec *time);
	enum timer_mode mode;
} hwapi_timer_t;

void* nanoclock_timer_run_thread(hwapi_timer_t* x);
void* timerfd_timer_run_thread(hwapi_timer_t* x);
void *timer_run_thread(void *timer);
int timer_setup(hwapi_timer_t *timer);
int timer_start(hwapi_timer_t *timer);
int timer_wait_period (hwapi_timer_t *timer);
void timer_close(hwapi_timer_t *timer);

#endif
