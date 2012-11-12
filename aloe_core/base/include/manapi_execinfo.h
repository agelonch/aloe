#ifndef MANAPI_EXECINFO_H_
#define MANAPI_EXECINFO_H_

#include "str.h"

/**
 * Inherits from General.ModExecInfo class. Part of the CMDAPI, adds functionality to retrieve module's ExecInfo attributes
 */

typedef struct {
	int module_ts;
	int node_ts;
	int mean_exec_us;
	int max_exec_us;
	int mean_rel_us;
	int max_rel_us;
	float mean_mopts;
	float max_mopts;
	time_t t_exec[3];
	int last_update_ts;
} execinfo_t;

#endif
