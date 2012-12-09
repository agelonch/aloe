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
#include "rtdal_itfqueue.h"
#include "rtdal.h"
#include "queue.h"


int rtdal_itfqueue_init(rtdal_itfqueue_t *obj) {
	rtdal_ASSERT_PARAM(obj);
	int i;
	char *x;

	obj->parent.is_external = 0;
	queue_init(&obj->queue,obj->max_msg_sz,obj->max_msg);
	obj->queue.id = obj->parent.id;

	return 0;
}

int rtdal_itfqueue_remove(h_itf_t obj) {
	rtdal_ASSERT_PARAM(obj);
	rtdal_itfqueue_t *itf = (rtdal_itfqueue_t*) obj;
	queue_free(&itf->queue);
	itf->max_msg = 0;
	itf->max_msg_sz = 0;
	itf->parent.id = 0;

	return 0;
}

int rtdal_itfqueue_send(h_itf_t obj, void* buffer, int len) {
	int n;
	rtdal_ASSERT_PARAM(obj);
	rtdal_ASSERT_PARAM(buffer);
	rtdal_ASSERT_PARAM(len>=0);
	rtdal_itfqueue_t *itf = (rtdal_itfqueue_t*) obj;
	h_pkt_t *pkt;

	if (len > itf->max_msg_sz) {
		rtdal_SETERROR(rtdal_ERROR_LARGE);
		return -1;
	}

	hdebug("requesting pkt for 0x%x\n",obj);
	if (!(pkt = rtdal_itfqueue_request_pkt(obj))) {
		return -1;
	}

	memcpy(pkt->data, buffer, (size_t) len);
	pkt->len = len;

	hdebug("put pkt for 0x%x pkt 0x%x\n",obj,pkt);
	n = rtdal_itfqueue_put_pkt(obj,pkt);
	if (n == -1) {
		return -1;
	} else if (n == 0) {
		return 0;
	}

	return 1;
}

int rtdal_itfqueue_recv(h_itf_t obj, void* buffer, int len) {
	rtdal_ASSERT_PARAM(obj);
	rtdal_ASSERT_PARAM(buffer);
	rtdal_ASSERT_PARAM(len>=0);
	h_pkt_t *pkt;

	if (!(pkt = rtdal_itfqueue_get_pkt(obj))) {
		return 0;
	}
	if (pkt->len > len) {
		pkt->len = len;
	}
	hdebug("obj=0x%x, rcv pkt=0x%x\n",obj,pkt);
	memcpy(buffer, pkt->data, (size_t) pkt->len);

	if (rtdal_itfqueue_release_pkt(obj,pkt) == -1) {
		return -1;
	}
	hdebug("release pkt 0x%x\n",pkt);

	return pkt->len;
}

int rtdal_itfqueue_set_callback(h_itf_t obj, void (*fnc)(void), int prio) {
	aerror("Not yet implemented");
	return -1;
}

int rtdal_itfqueue_set_blocking(h_itf_t obj, int block) {
	aerror("Not yet implemented");
	return -1;
}

int rtdal_itfqueue_get_blocking(h_itf_t obj) {
	aerror("Not yet implemented");
	return -1;
}

h_pkt_t* rtdal_itfqueue_request_pkt(h_itf_t obj) {
	rtdal_ASSERT_PARAM_P(obj);
	rtdal_itfqueue_t *itf = (rtdal_itfqueue_t*) obj;
	h_pkt_t* pkt;

	pkt = queue_request(&itf->queue);
	if (!pkt) {
		aerror("no more packets\n");
		rtdal_SETERROR(rtdal_ERROR_NOSPACE);
		return NULL;
	}
	return pkt;
}

int rtdal_itfqueue_put_pkt(h_itf_t obj, h_pkt_t* pkt) {
	rtdal_ASSERT_PARAM(obj);
	rtdal_itfqueue_t *itf = (rtdal_itfqueue_t*) obj;

	pkt->tstamp = rtdal_time_slot()+1;

	if (queue_push(&itf->queue)) {
		rtdal_SETERROR(rtdal_ERROR_NOSPACE);
		return 0;
	}

	return 1;
}

h_pkt_t* rtdal_itfqueue_get_pkt(h_itf_t obj) {
	rtdal_ASSERT_PARAM_P(obj);
	rtdal_itfqueue_t *itf = (rtdal_itfqueue_t*) obj;

	h_pkt_t* pkt;

	pkt = queue_pop(&itf->queue,rtdal_time_slot());
	if (!pkt) {
		return NULL;
	}
	return pkt;
}

int rtdal_itfqueue_release_pkt(h_itf_t obj, h_pkt_t *pkt) {
	rtdal_ASSERT_PARAM(obj);
	rtdal_itfqueue_t *itf = (rtdal_itfqueue_t*) obj;

	if (queue_release(&itf->queue)) {
		aerror("can't release packet\n");
		rtdal_SETERROR(rtdal_ERROR_NOSPACE);
		return 0;
	}

	return 1;
}

int rtdal_itfqueue_set_delay(h_itf_t obj, int delay) {
	aerror("Not yet implemented");
	return -1;
}

int rtdal_itfqueue_get_delay(h_itf_t obj) {
	aerror("Not yet implemented");
	return -1;
}

