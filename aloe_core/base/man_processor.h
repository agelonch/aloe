#ifndef MAN_PROCESSOR_H
#define MAN_PROCESSOR_H

typedef struct {

	/**
	 * Total processor capacity, in MOPS
	 */
	float total_c;
	/**
	 * Total internal processor bandwidth (i.e. memory bandwidth). In MBPS
	 */
	float total_b;
	/**
	 * Remaining processor capacity, in MOPS
	 */
	float rem_c;
	/**
	 * Remaining internal bandwidth (in MBPS)
	 */
	float rem_b;
	/**
	 * Processor index in the node
	 */
	int idx;

} man_processor_t;

#endif

