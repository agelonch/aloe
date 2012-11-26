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
#include "hwapi_itfqueue.h"
#include "hwapi_itfphysic.h"
#include "hwapi.h"

#define call(a, ...) if (obj->is_external) return hwapi_itfphysic_##a(__VA_ARGS__); else return hwapi_itfqueue_##a(__VA_ARGS__)

int hwapi_itf_remove(h_itf_t obj) {
	hdebug("type=%s, obj=0x%x\n",obj->is_external?"external":"internal",obj);
	call(remove,obj);
}

int hwapi_itf_recv(h_itf_t obj, void* buffer, int len) {
	hdebug("type=%s, obj=0x%x, buffer=0x%x, len=%d\n",obj->is_external?"external":"internal",obj,
			buffer,len);
	call(recv,obj,buffer,len);
}

int hwapi_itf_send(h_itf_t obj, void* buffer, int len) {
	hdebug("type=%s, obj=0x%x, buffer=0x%x, len=%d\n",obj->is_external?"external":"internal",obj,
			buffer,len);
	call(send,obj,buffer,len);
}

int hwapi_itf_set_blocking(h_itf_t obj, int block) {
	hdebug("type=%s, obj=0x%x, block=%d\n",obj->is_external?"external":"internal",obj,
			block);
	call(set_blocking,obj,block);
}

int hwapi_itf_get_blocking(h_itf_t obj) {
	hdebug("type=%s, obj=0x%x\n",obj->is_external?"external":"internal",obj);
	call(get_blocking,obj);
}

pkt_t* hwapi_itf_request_pkt(h_itf_t obj) {
	hdebug("type=%s, obj=0x%x\n",obj->is_external?"external":"internal",obj);
	call(request_pkt,obj);
}

int hwapi_itf_release_pkt(h_itf_t obj, pkt_t* pkt) {
	hdebug("type=%s, obj=0x%x, pkt=0x%x\n",obj->is_external?"external":"internal",obj,pkt);
	call(release_pkt,obj,pkt);
}

int hwapi_itf_put_pkt(h_itf_t obj, pkt_t* pkt) {
	hdebug("type=%s, obj=0x%x, pkt=0x%x\n",obj->is_external?"external":"internal",obj,pkt);
	call(put_pkt,obj,pkt);
}

int hwapi_itf_set_callback(h_itf_t obj, void (*fnc)(void), int prio) {
	hdebug("type=%s, obj=0x%x, fnc=0x%x, prio=%d\n",obj->is_external?"external":"internal",obj,
			fnc,prio);
	call(set_callback,obj,fnc,prio);
}

int hwapi_itf_set_delay(h_itf_t obj, int delay) {
	hdebug("type=%s, delay=%d\n",obj->is_external?"external":"internal",obj);
	call(set_delay,obj,delay);
}

int hwapi_itf_get_delay(h_itf_t obj) {
	hdebug("type=%s, obj=0x%x\n",obj->is_external?"external":"internal",obj);
	call(get_delay,obj);
}

pkt_t* hwapi_itf_get_pkt(h_itf_t obj) {
	hdebug("type=%s, obj=0x%x\n",obj->is_external?"external":"internal",obj);
	call(get_pkt,obj);
}
