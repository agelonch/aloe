#ifndef MANAPI_MODULE_H_
#define MANAPI_MODULE_H_

#include "str.h"
#include "manapi_variable.h"
#include "manapi_execinfo.h"
#include "manapi_interface.h"

/**
 * Inherits from General.Module class. Part of the CMDAPI, used to update module's variables or execinfo information
 */
typedef struct {

	variable_t variables[MAX(variables)];
	interface_t interfaces[MAX(interfaces)];
	execinfo_t execinfo;
	void *node;

	int id;
	int last_update_ts;
	strdef(name);

	/**
	 * name of the binary, not the full path. Depending on the platform it can be a stand-alone executable (program) or a library which is dynamically loaded.
	 */
	strdef(binary);

	/**
	 * Module processing requirements in MOPTS
	 */
	float c_mopts;

	/**
	 * Processor id where the module has been mapped.
	 */
	int processor_idx;
	int status;
	int nof_reporting_vars;
	int nof_variables;
	int nof_interfaces;

} module_t;

#endif
