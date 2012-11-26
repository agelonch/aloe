#include <stdio.h>

#include "swapi.h"

int Init(void *context) {
	printf("tx is in init\n");
	return 0;
}

int Run(void *context) {
	printf("tx is in run\n");
	return 0;
}

int Stop(void *context) {
	printf("tx is stop\n");
	return 0;
}
