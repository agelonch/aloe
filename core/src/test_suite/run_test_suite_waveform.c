#include <stdlib.h>
#include "defs.h"

#include "manapi.h"
#include "waveform.h"
#include "nod_waveform.h"
#include "mempool.h"

waveform_t waveform;

void *run_test_suite_waveform(void *arg) {
	int c;
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

	waveform_status_t new_status;
	do {
		fflush(stdout);
		printf("\n\nType the new status ('l'=Load, 'i'=Init, 'r'=Run, 'p'=Pause, 't'=Step, 's'=Stop) or Ctrl+C to exit\n");
		c = getchar();
		new_status.cur_status = LOADED;
		switch((char) c) {
		case 'l':
			if (waveform_load(&waveform)) {
				aerror("loading waveform\n");
				return NULL;
			}
			fflush(stdout);
			printf("Waveform LOADED!\n");
			break;
		case 'i':
			new_status.cur_status=INIT;
			break;
		case 'r':
			new_status.cur_status=RUN;
			break;
		case 'p':
			new_status.cur_status=PAUSE;
			break;
		case 't':
			new_status.cur_status=STEP;
			break;
		case 's':
			new_status.cur_status=STOP;
			break;
		default:
			printf("Unknown command %c\n",(char) c);
			break;
		}
		if (new_status.cur_status != LOADED) {
			new_status.dead_timeslot = hwapi_time_slot() + 5;
			new_status.next_timeslot = hwapi_time_slot() + 2;
			if (waveform_status_set(&waveform,&new_status)) {
				printf("DID NOT CHANGE!\n");
			} else {
				printf("DONE!\n");
			}
		}
	} while(1);
	/* status init */

	/* status run */
	printf("Going run\n");
	new_status.cur_status = RUN;
	new_status.dead_timeslot = hwapi_time_slot() + 5;
	new_status.next_timeslot = hwapi_time_slot() + 2;
	if (waveform_status_set(&waveform,&new_status)) {
		aerror("setting status\n");
		return NULL;
	}

	/* pause o sleep o return */

	printf("exit\n");
	return NULL;
}
