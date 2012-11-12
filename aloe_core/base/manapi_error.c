#include <stddef.h>
#include <stdio.h>
#include "str.h"
#include "manapi_context.h"
#include "manapi_error.h"
#include "manapi.h"

static manapi_error_t *context;

void manapi_error_set_context(manapi_error_t *_context) {
	context = _context;
}

static strdef(error_buffer);

void manapi_error_hwapi(const char *msg) {
	manapi_error_set(0, msg);
}

void manapi_error_set(manapi_errorcodes_t code, const char *msg) {
	context->code=code;
	strcpy(context->msg, msg);
}

void manapi_error_print(void *ctx) {
	fprintf(stderr, "[file %s, line %d]: %s",__FILE__, __LINE__, manapi_error_string(ctx));
}

string manapi_error_string(void *ctx) {
	switch(context->code) {
	case 0:
		sprintf(error_buffer, "[hwapi error: %s]",hwapi_error_string());
		break;
	default:
		sprintf(error_buffer, "uknown error: ");
		break;
	}
	sprintf(error_buffer, "%s %s\n", error_buffer, context->msg);
	return error_buffer;
}

int manapi_error_code() {
	return (int) context->code;
}
