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

#include <stdlib.h>
#include <stddef.h>
#include <errno.h>
#include "defs.h"
#include "str.h"
#include "rtdal_error.h"
#include "rtdal_itf.h"
#include "rtdal_itfspscq.h"
#include "rtdal.h"

#define cast(a,b) RTDAL_ASSERT_PARAM(a);\
			rtdal_itfspscq_t *b = (rtdal_itfspscq_t*) a;
#define cast_p(a,b) RTDAL_ASSERT_PARAM_p(a);\
			rtdal_itfspscq_t *b = (rtdal_itfspscq_t*) a;

int rtdal_itfspscq_init(rtdal_itfspscq_t *itf) {
	RTDAL_ASSERT_PARAM(itf);
	int i;
	char *x;

	itf->parent.is_external = 0;
	itf->parent.delay = 1;
	itf->read = 0;
	itf->write = 0;
	itf->data = malloc(itf->max_msg*itf->max_msg_sz);
	itf->packets = calloc(itf->max_msg,sizeof(r_pkt_t));
	if (!itf->data || !itf->packets) {
		return -1;
	}
	for (i=0;i<itf->max_msg;i++) {
		itf->packets[i].data = &itf->data[i*itf->max_msg_sz];
		itf->packets[i].valid = 0;
	}

	return 0;
}

int rtdal_itfspscq_remove(r_itf_t obj) {
	cast(obj,itf);
	if (itf->data) {
		free(itf->data);
		itf->data = NULL;
	}
	if (itf->packets) {
		free(itf->packets);
		itf->packets = NULL;
	}
	itf->max_msg = 0;
	itf->max_msg_sz = 0;
	itf->parent.id = 0;

	return 0;
}


inline static int spscq_is_empty(rtdal_itfspscq_t *itf) {
	return (itf->packets[itf->read].valid == 0);
}

inline static int spscq_is_full(rtdal_itfspscq_t *itf) {
	return (itf->packets[itf->write].valid != 0);
}

int rtdal_itfspscq_request(r_itf_t obj, void **ptr) {
	cast(obj,itf);
	RTDAL_ASSERT_PARAM(ptr);

	*ptr = NULL;
	qdebug("write=%d, tstamp=%d\n",itf->write,itf->packets[itf->write].tstamp);

	if (spscq_is_full(itf)) {
		RTDAL_SETERROR(RTDAL_ERROR_NOSPACE);
		return 0;
	}

	qdebug("[ok] write=%d/%d, addr=0x%x\n",itf->write,itf->max_msg,
			itf->packets[itf->write].data);
	*ptr = itf->packets[itf->write].data;

	return 1;
}

int rtdal_itfspscq_push(r_itf_t obj, int len) {
	cast(obj,itf);
	if (spscq_is_full(itf)) {
		RTDAL_SETERROR(RTDAL_ERROR_NOSPACE);
		return 0;
	}
	itf->packets[itf->write].tstamp = rtdal_time_slot()+itf->parent.delay;
	itf->packets[itf->write].len = len;
	itf->packets[itf->write].valid = 1;
	itf->write += (itf->write+1 >= itf->max_msg) ? (1-itf->max_msg) : 1;

	qdebug("write=%d/%d, len=%d\n",itf->write,itf->max_msg,len);

	return 1;
}

int rtdal_itfspscq_pop(r_itf_t obj, void **ptr, int *len) {
	cast(obj,itf);
	RTDAL_ASSERT_PARAM(ptr);
	RTDAL_ASSERT_PARAM(len);

	*ptr = NULL;
	*len = 0;

	int tstamp = rtdal_time_slot();
	if (spscq_is_empty(itf)) {
		qdebug("[empty] read=%d, tstamp=%d\n",itf->read,itf->packets[itf->read].tstamp);
		return 0;
	}
	if (itf->packets[itf->read].tstamp > tstamp) {
		qdebug("[delay] read=%d, tstamp=%d\n",itf->read,itf->packets[itf->read].tstamp);
		return 0;
	}
	if (itf->packets[itf->read].tstamp < tstamp) {
		qdebug("[late] read=%d tstamp=%d ara=%d\n",itf->read,itf->packets[itf->read].tstamp,tstamp);
	}

	qdebug("[ok] read=%d, tstamp=%d (now=%d)\n",itf->read,itf->packets[itf->read].tstamp,tstamp);
	*ptr = itf->packets[itf->read].data;
	*len = itf->packets[itf->read].len;

	return 1;
}

int rtdal_itfspscq_release(r_itf_t obj) {
	cast(obj,itf);
	if (spscq_is_empty(itf)) {
		qdebug("[empty] read=%d, tstamp=%d\n",itf->read,itf->packets[itf->read].tstamp);
		return 0;
	}

	qdebug("read=%d, tstamp=%d\n",itf->read,itf->packets[itf->read].tstamp);
	itf->packets[itf->read].valid = 0;
	itf->read += (itf->read+1 >= itf->max_msg) ? (1-itf->max_msg) : 1;

	return 1;
}

int rtdal_itfspscq_set_delay(r_itf_t obj, int delay) {
	aerror("Not yet implemented");
	return -1;
}

int rtdal_itfspscq_get_delay(r_itf_t obj) {
	aerror("Not yet implemented");
	return -1;
}


int rtdal_itfspscq_set_callback(r_itf_t obj, void (*fnc)(void), int prio) {
	aerror("Not yet implemented");
	return -1;
}

int rtdal_itfspscq_set_blocking(r_itf_t obj, int block) {
	aerror("Not yet implemented");
	return -1;
}

int rtdal_itfspscq_get_blocking(r_itf_t obj) {
	aerror("Not yet implemented");
	return -1;
}


int rtdal_itfspscq_send(r_itf_t obj, void* buffer, int len) {
	cast(obj,itf);
	RTDAL_ASSERT_PARAM(buffer);
	RTDAL_ASSERT_PARAM(len>=0);

	int n;
	void *ptr;

	if (len > itf->max_msg_sz) {
		RTDAL_SETERROR(RTDAL_ERROR_LARGE);
		return -1;
	}

	hdebug("requesting pkt for 0x%x\n",obj);
	if ((n = rtdal_itfspscq_request(obj, &ptr)) != 1) {
		return n;
	}

	memcpy(ptr, buffer, (size_t) len);

	hdebug("put pkt for 0x%x pkt 0x%x\n",obj,ptr);
	return rtdal_itfspscq_push(obj,len);
}

int rtdal_itfspscq_recv(r_itf_t obj, void* buffer, int len) {
	cast(obj,itf);

	RTDAL_ASSERT_PARAM(buffer);
	RTDAL_ASSERT_PARAM(len>=0);

	int n, plen;
	void *ptr;

	if ((n = rtdal_itfspscq_pop(obj, &ptr, &plen)) != 1) {
		return n;
	}
	if (plen > len) {
		plen = len;
	}

	hdebug("obj=0x%x, rcv pkt=0x%x\n",obj,ptr);
	memcpy(buffer, ptr, (size_t) plen);

	if ((n = rtdal_itfspscq_release(obj)) == 1) {
		return n;
	}

	hdebug("release pkt 0x%x\n",ptr);

	return plen;
}


