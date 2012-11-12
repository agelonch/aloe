#ifndef MANAPI_CONTEXT_H_
#define MANAPI_CONTEXT_H_

#include "man_platform.h"
#include "manapi_error.h"
#include "objects_max.h"

typedef struct {
	waveform_t waveformsArray[MAX(waveforms)];
	manapi_error_t error;
	man_platform_t platform;
}manapi_context_t;

#endif
