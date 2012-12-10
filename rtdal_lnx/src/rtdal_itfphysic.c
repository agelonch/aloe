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
#include "defs.h"
#include "str.h"
#include "rtdal_itf.h"
#include "rtdal_itfphysic.h"
#include "rtdal.h"

int rtdal_itfphysic_create(r_itf_t obj, string address) {
	aerror("Not yet implemented");
	return -1;
}

int rtdal_itfphysic_connect(r_itf_t obj) {
	aerror("Not yet implemented");
	return -1;
}

int rtdal_itfphysic_disconnect(r_itf_t obj) {
	aerror("Not yet implemented");
	return -1;
}

int rtdal_itfphysic_remove(r_itf_t obj) {
	aerror("Not yet implemented");
	return -1;
}

int rtdal_itfphysic_recv(r_itf_t obj, void* buffer, int len) {
	aerror("Not yet implemented");
	return -1;
}

int rtdal_itfphysic_send(r_itf_t obj, void* buffer, int len) {
	aerror("Not yet implemented");
	return -1;
}

int rtdal_itfphysic_set_blocking(r_itf_t obj, int block) {
	aerror("Not yet implemented");
	return -1;
}

int rtdal_itfphysic_get_blocking(r_itf_t obj) {
	aerror("Not yet implemented");
	return -1;
}

int rtdal_itfphysic_request(r_itf_t obj, void **ptr) {
	aerror("Not yet implemented");
	return -1;
}

int rtdal_itfphysic_release(r_itf_t obj) {
	aerror("Not yet implemented");
	return -1;
}

int rtdal_itfphysic_push(r_itf_t obj, int len) {
	aerror("Not yet implemented");
	return -1;
}

int rtdal_itfphysic_pop(r_itf_t obj, void **ptr, int *len) {
	aerror("Not yet implemented");
	return -1;
}

int rtdal_itfphysic_set_callback(r_itf_t obj, void (*fnc)(void), int prio) {
	aerror("Not yet implemented");
	return -1;
}

int rtdal_itfphysic_set_delay(r_itf_t obj, int delay) {
	aerror("Not yet implemented");
	return -1;
}

int rtdal_itfphysic_get_delay(r_itf_t obj) {
	return -1;//rtdal_itf_get_delay__(obj);
}

