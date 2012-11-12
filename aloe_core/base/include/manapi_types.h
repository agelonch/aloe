#ifndef MANAPI_TYPES_H_
#define MANAPI_TYPES_H_

#include "str.h"
#include "objects_max.h"
#include "hwapi.h"
#include "manapi_execinfo.h"
#include "manapi_interface.h"
#include "manapi_module.h"
#include "manapi_variable.h"
#include "manapi_waveform.h"

typedef void updateable;


typedef struct {
	float C[MAX(processors)];
	float B[MAX(processors)];
	float I[MAX(processors)][MAX(processors)];
	float C_rem[MAX(processors)];
	float B_rem[MAX(processors)];
} man_platform_model_t;








#endif
