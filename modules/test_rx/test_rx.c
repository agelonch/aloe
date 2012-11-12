#include <stdio.h>

#include "hwapi.h"
#include "hwapi_error.h"

static h_itf_t itf = NULL;
int Run(void *context) {
	int r,n;
	if (!itf) {
		itf = (h_itf_t) hwapi_itfqueue_new("test",4,16);
		if (!itf) {
			hwapi_perror("hwapi_itfqueue_new");
			return -1;
		}
	} else {
		n = hwapi_itfqueue_recv(itf, &r, 4);
		if (n == -1) {
			hwapi_perror("hwapi_rcv");
			return -1;
		}
		if (!n) {
			printf("Recv 0\n");
		} else {
			printf("recv %d\n",r);
		}
	}
	return 0;
}
