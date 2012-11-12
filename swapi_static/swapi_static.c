#include <stdio.h>
#include "defs.h"
#include "swapi_static.h"


int _run_cycle(void* context) {
	return Run(context);
}

void _abort(void* context) {
	aerror("Not yet implemented");
}
