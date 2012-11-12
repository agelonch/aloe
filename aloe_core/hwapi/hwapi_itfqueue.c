#include <stdlib.h>
#include <stddef.h>

#include "defs.h"
#include "str.h"
#include "hwapi_error.h"
#include "hwapi_itf.h"
#include "hwapi_itfqueue.h"
#include "hwapi.h"
#include "queue.h"


int hwapi_itfqueue_init(hwapi_itfqueue_t *obj) {
	HWAPI_ASSERT_PARAM(obj);
	int i;
	char *x;

	if (obj->max_msg > MAX_QUEUE_SZ) {
		HWAPI_SETERROR(HWAPI_ERROR_LARGE);
		return -1;
	}

	/** todo: use static memory allocation */
	obj->data = malloc((size_t) obj->max_msg * (size_t) obj->max_msg_sz);
	if (!obj->data) {
		HWAPI_SYSERROR("malloc");
		return -1;
	}
	obj->packets = malloc(sizeof(h_pkt_t)*(size_t) obj->max_msg);
	if (!obj->packets) {
		HWAPI_SYSERROR("malloc");
		return -1;
	}

	queue_init(&obj->q_tx);
	queue_init(&obj->q_pkts);

	x = obj->data;
	for (i=0;i<obj->max_msg;i++) {
		obj->packets[i].data = &x[obj->max_msg_sz*i];
		obj->packets[i].len = 0;
		queue_put(&obj->q_pkts, &obj->packets[i]);
	}

	return 0;
}

void hwapi_itfqueue_destroy(hwapi_itfqueue_t *obj) {
	assert(obj);

	free(obj->data);
}

int hwapi_itfqueue_send(h_itf_t obj, void* buffer, int len) {
	int n;
	HWAPI_ASSERT_PARAM(obj);
	HWAPI_ASSERT_PARAM(buffer);
	HWAPI_ASSERT_PARAM(len>=0);
	hwapi_itfqueue_t *itf = (hwapi_itfqueue_t*) obj;
	h_pkt_t *pkt;

	if (len > itf->max_msg_sz) {
		HWAPI_SETERROR(HWAPI_ERROR_LARGE);
		return -1;
	}

	if (!(pkt = hwapi_itfqueue_request_pkt(obj))) {
		return -1;
	}

	memcpy(pkt->data, buffer, (size_t) len);
	pkt->len = len;

	n = hwapi_itfqueue_put_pkt(obj,pkt);
	if (n == -1) {
		return -1;
	} else if (n == 0) {
		return 0;
	}

	return 1;
}

int hwapi_itfqueue_recv(h_itf_t obj, void* buffer, int len) {
	HWAPI_ASSERT_PARAM(obj);
	HWAPI_ASSERT_PARAM(buffer);
	HWAPI_ASSERT_PARAM(len>=0);
	h_pkt_t *pkt;
	int n;

	if (!(pkt = hwapi_itfqueue_get_pkt(obj))) {
		return 0;
	}
	if (pkt->len > len) {
		pkt->len = len;
	}

	memcpy(buffer, pkt->data, (size_t) pkt->len);

	if (hwapi_itfqueue_release_pkt(obj,pkt) == -1) {
		return -1;
	}

	return pkt->len;
}

int hwapi_itfqueue_set_callback(h_itf_t obj, void (*fnc)(void), int prio) {
	aerror("Not yet implemented");
	return -1;
}

int hwapi_itfqueue_set_blocking(h_itf_t obj, int block) {
	aerror("Not yet implemented");
	return -1;
}

int hwapi_itfqueue_get_blocking(h_itf_t obj) {
	aerror("Not yet implemented");
	return -1;
}

h_pkt_t* hwapi_itfqueue_request_pkt(h_itf_t obj) {
	HWAPI_ASSERT_PARAM_P(obj);
	hwapi_itfqueue_t *itf = (hwapi_itfqueue_t*) obj;

	return (h_pkt_t*) queue_get(&itf->q_pkts);
}

int hwapi_itfqueue_put_pkt(h_itf_t obj, h_pkt_t* pkt) {
	HWAPI_ASSERT_PARAM(obj);
	hwapi_itfqueue_t *itf = (hwapi_itfqueue_t*) obj;

	if (queue_put(&itf->q_tx, pkt)) {
		return 0;
	}

	return 1;
}

h_pkt_t* hwapi_itfqueue_get_pkt(h_itf_t obj) {
	HWAPI_ASSERT_PARAM_P(obj);
	hwapi_itfqueue_t *itf = (hwapi_itfqueue_t*) obj;

	return queue_get(&itf->q_tx);
}

int hwapi_itfqueue_release_pkt(h_itf_t obj, h_pkt_t *pkt) {
	HWAPI_ASSERT_PARAM(obj);
	hwapi_itfqueue_t *itf = (hwapi_itfqueue_t*) obj;

	if (queue_put(&itf->q_pkts, pkt)) {
		return 0;
	}

	return 1;
}

int hwapi_itfqueue_set_delay(h_itf_t obj, int delay) {
	aerror("Not yet implemented");
	return -1;
}

int hwapi_itfqueue_get_delay(h_itf_t obj) {
	aerror("Not yet implemented");
	return -1;
}

