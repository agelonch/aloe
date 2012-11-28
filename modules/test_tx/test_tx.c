#include <stdio.h>

#include "swapi.h"

int cnt;

int Init(void *context) {
	printf("tx is in init\n");
	return 1;
}

int Run(void *context) {
	cnt++;
	while(1)
		cnt++;
	printf("tx is in run cnt=%d\n",cnt);
	return -1;
}

int Stop(void *context) {
	printf("tx is stop\n");
	swapi_exit(context);
	return 0;
}
