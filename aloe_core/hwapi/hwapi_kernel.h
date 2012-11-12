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

#ifndef HWAPI_KERNEL_H
#define HWAPI_KERNEL_H

#define THREADED_CPU

#define ALOE_VERSION "ALOE++-0.1"
#define ALOE_YEAR 2012

#ifdef THREADED_CPU
#define CORE_ID(i) 2*i+1
#else
#define CORE_ID(i) i
#endif

void hwapi_kernel_tasks();
int hwapi_kernel_sigwait_thread();

#endif
