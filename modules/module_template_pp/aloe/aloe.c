#include <stddef.h>
#include "params.h"
//#include "stats.h"
#include "modulename_params.h"


/**\brief	This function must be implemented by the DSP module. It return a pointer to a buffer
 * of nof_variables structures of type variable_t. The function stores in nof_variables pointer
 * the number of variables to configure. Returns null if there are no variables to configure.
 */
user_var_t*		variables_list(int *nof_variables) {
	/*@TODO: implement */
	*nof_variables = 0;
	return NULL;
}

/**\brief	This function must be implemented by the DSP module. It return a pointer to a buffer
 * of nof_parameters structures of type param_t. The function stores in nof_paramters pointer
 * the number of parameters to configure. Returns null if there are no parameters to configure.
 */
param_t*		param_list(int *nof_params) {
	*nof_params = nof_parameters;
	return parameters;
}
