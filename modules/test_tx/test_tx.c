#include <stdio.h>

#include "hwapi.h"
#include "hwapi_error.h"

static h_itf_t itf = NULL;
static int cnt = 0;
int Run(void *context) {
	int n;
	if (!itf) {
		itf = (h_itf_t) hwapi_itfqueue_get("test");
	} else {
		cnt++;
		n = hwapi_itfqueue_send(itf, &cnt, 4);
		if (n == -1) {
			hwapi_perror("hwapi_send");
			return -1;
		}
		if (!n) {
			printf("Send 0\n");
		} else {
			printf("Send %d\n",cnt);
		}
	}
	return 0;
}
