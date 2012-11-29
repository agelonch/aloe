#include <stdio.h>

#include "swapi.h"

int cnt;

itf_t output;
int Init(void *context) {
	printf("tx is in init\n");
	output = swapi_itf_create(context, 0, ITF_WRITE, 229376);
	if (!output) {
		swapi_error_print(context,"itf_create");
		return -1;
	}
	return 1;
}

int x[20];
int Run(void *context) {
	cnt++;
	int i,n;
	for (i=0;i<20;i++) {
		x[i]=cnt+i;
	}
	n=swapi_itf_write(output,x,20*sizeof(int));
	printf("tx sends %d, cnt=%d\n",n,cnt);
	return 0;
}

int Stop(void *context) {
	printf("tx is stop\n");
	swapi_exit(context);
	return 0;
}
