#ifndef MANAPI_ERROR_H
#define MANAPI_ERROR_H

#include "str.h"

typedef enum {
	INVAL,NOSPACE, EXISTS, HWAPI, BIG, OTHER
} manapi_errorcodes_t;

typedef struct {
	manapi_errorcodes_t code;
	strdef(msg);
}manapi_error_t;

void manapi_error_set_context(manapi_error_t *context);
void manapi_error_hwapi(const char *msg);
void manapi_error_set(manapi_errorcodes_t code, const char *msg);

#endif
