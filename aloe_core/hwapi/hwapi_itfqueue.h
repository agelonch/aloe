#ifndef HWAPI_ITFQUEUE_H
#define HWAPI_ITFQUEUE_H

#include "str.h"
#include "hwapi_itf.h"
#include "hwapi.h"
#include "queue.h"

typedef struct {
	hwapi_itf_t parent;

	int max_msg;
	int max_msg_sz;

	queue_t q_tx;
	queue_t q_pkts;

	h_pkt_t *packets;
	void *data;
}hwapi_itfqueue_t;

int hwapi_itfqueue_init(hwapi_itfqueue_t *obj);

#endif
