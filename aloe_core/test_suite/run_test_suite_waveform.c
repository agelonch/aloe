#include <stdlib.h>
#include "defs.h"

#include "manapi.h"
#include "waveform.h"
#include "nod_waveform.h"
#include "mempool.h"

waveform_t waveform;

void *run_test_suite_waveform(void *arg) {

	strcpy(waveform.model_file,arg);
	strcpy(waveform.name,arg);

	/* this will be done by the module */
	if (nod_anode_initialize(2)) {
		aerror("initializing node\n");
		return NULL;
	}

	/* from here, this is the MANAPI interface only */
	if (manapi_initialize(NULL,2)) {
		aerror("initializing manapi\n");
		return NULL;
	}

	if (waveform_parse(&waveform)) {
		aerror("parsing waveform\n");
		return NULL;
	}

	printf("Loading..\n");

	if (waveform_load(&waveform)) {
		aerror("loading waveform\n");
		return NULL;
	}

	printf("Going init\n");
	waveform_status_t new_status;
	new_status.cur_status = INIT;
	new_status.dead_timeslot = hwapi_time_slot() + 5;
	new_status.next_timeslot = hwapi_time_slot() + 2;
	if (waveform_status_set(&waveform,&new_status)) {
		aerror("setting status\n");
		return NULL;
	}
	/* status init */

	/* status run */
	printf("Going run\n");

	/* pause o sleep o return */

	printf("exit\n");
	return NULL;
}
