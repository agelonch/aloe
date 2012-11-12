#ifndef SWAPI_ERROR_H
#define SWAPI_ERROR_H

#include "str.h"

typedef enum {
	INVAL, NOSPACE, EXISTS, HWAPI,BIG, OTHER
}swapi_errorcodes_t;

typedef struct {
	int id;
	swapi_errorcodes_t code;
	strdef(msg);
}swapi_error_t;

void swapi_error_hwapi(void *ctx, const char *msg);
void swapi_error_set(void *ctx, swapi_errorcodes_t code, const char *msg);

#endif
