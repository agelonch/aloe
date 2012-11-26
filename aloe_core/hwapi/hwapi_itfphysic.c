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
#include "hwapi_itf.h"
#include "hwapi_itfphysic.h"
#include "hwapi.h"

int hwapi_itfphysic_create(h_itf_t obj, string address) {
	aerror("Not yet implemented");
	return -1;
}

int hwapi_itfphysic_connect(h_itf_t obj) {
	aerror("Not yet implemented");
	return -1;
}

int hwapi_itfphysic_disconnect(h_itf_t obj) {
	aerror("Not yet implemented");
	return -1;
}

int hwapi_itfphysic_remove(h_itf_t obj) {
	aerror("Not yet implemented");
	return -1;
}

int hwapi_itfphysic_recv(h_itf_t obj, void* buffer, int len) {
	aerror("Not yet implemented");
	return -1;
}

int hwapi_itfphysic_send(h_itf_t obj, void* buffer, int len) {
	aerror("Not yet implemented");
	return -1;
}

int hwapi_itfphysic_set_blocking(h_itf_t obj, int block) {
	aerror("Not yet implemented");
	return -1;
}

int hwapi_itfphysic_get_blocking(h_itf_t obj) {
	aerror("Not yet implemented");
	return -1;
}

pkt_t* hwapi_itfphysic_request_pkt(h_itf_t obj) {
	aerror("Not yet implemented");
	return NULL;
}

int hwapi_itfphysic_release_pkt(h_itf_t obj, pkt_t* pkt) {
	aerror("Not yet implemented");
	return -1;
}

int hwapi_itfphysic_put_pkt(h_itf_t obj, pkt_t* pkt) {
	aerror("Not yet implemented");
	return -1;
}

int hwapi_itfphysic_set_callback(h_itf_t obj, void (*fnc)(void), int prio) {
	aerror("Not yet implemented");
	return -1;
}

int hwapi_itfphysic_set_delay(h_itf_t obj, int delay) {
	aerror("Not yet implemented");
	return -1;
}

int hwapi_itfphysic_get_delay(h_itf_t obj) {
	return -1;//hwapi_itf_get_delay__(obj);
}

pkt_t* hwapi_itfphysic_get_pkt(h_itf_t obj) {
	aerror("Not yet implemented");
	return NULL;
}
