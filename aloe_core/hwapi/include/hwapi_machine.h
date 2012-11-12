#ifndef HWAPI_MACHINE_H
#define HWAPI_MACHINE_H

#include "str.h"

enum clock_source {
	SINGLE_TIMER, SYNC_SLAVE, MULTI_TIMER, DAC
};

#define RT_FAULT_OPTS_HARD 1
#define RT_FAULT_OPTS_SOFT 2


/**
 * Public structure configured at initialize() from the information read from platform.conf. Stores some properties of the local machine architecture.
 */
typedef struct {
	long int ts_len_us;
	int cpu_type;
	float mopts;
	float mbpts;
	string name;
	int nof_processors;
	int rt_fault_opts;
	int kernel_prio;
	int pipeline_prio;
	int sync_period;
	int sync_continuous;
	int slave_master;
	int max_waveforms;
	int max_modules_x_waveform;
	int max_variables_x_module;
	enum clock_source clock_source;
	void (*slave_sync_kernel) (void*, struct timespec *time);
}hwapi_machine_t;

#endif
