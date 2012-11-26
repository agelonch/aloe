#include <stdio.h>

#include "swapi.h"

int Init(void *context) {
	printf("rx is in init\n");
	return 0;
}

int Run(void *context) {
	printf("rx is in run\n");
	return 0;
}

int Stop(void *context) {
	printf("rx is stop\n");
	return 0;
}
