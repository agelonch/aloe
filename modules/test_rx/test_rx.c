#include <stdio.h>

#include "swapi.h"

int cnt=0;

int Init(void *context) {
	printf("rx is in init\n");
	return 1;
}

int Run(void *context) {
	cnt++;
	while(1)
		cnt++;
	printf("rx is in run\n");
	return 0;
}

int Stop(void *context) {
	printf("rx is stop\n");
	swapi_exit(context);
	return 0;
}
