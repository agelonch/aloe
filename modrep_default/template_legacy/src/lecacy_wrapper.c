
#include <stdlib.h>
#include <aloe/params.h>
#include "templatelegacy_params.h"

param_t *param_list(int *nof_p) {
	*nof_p = nof_parameters;
	return parameters;
}
