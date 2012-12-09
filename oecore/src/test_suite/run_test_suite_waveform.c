#include <stdlib.h>
#include "defs.h"

#include "manapi.h"
#include "waveform.h"
#include "nod_waveform.h"
#include "mempool.h"
#include "hwapi.h"

/* in a multi-processing environment, this file wont be included */
#include "nod_anode.h"

waveform_t waveform;

int print_execinfo(waveform_t *waveform, int tslot_us) {
	int i;
	const char *t;
	int total_cpu=0;
	printf("\t========================= Execinfo: %s ==============\n\n",waveform->name);
	printf("\tName\t\t  Mean Exec (us)   Max Exec (us) Mean Ini -> End (us)  Max Ini -> End (us)\n");
	for (i=0;i<waveform->nof_modules;i++) {
		if (strlen(waveform->modules[i].name)>8) {
			t="\t";
		} else {
			t="\t\t";
		}
		printf("\t%s%s%16.2f%16d%8.2f -> %8.2f  %8d -> %5d\n",waveform->modules[i].name,t,
				waveform->modules[i].execinfo.mean_exec_us,waveform->modules[i].execinfo.max_exec_us,
				waveform->modules[i].execinfo.mean_start_us,waveform->modules[i].execinfo.mean_rel_us,
				waveform->modules[i].execinfo.max_start_us,waveform->modules[i].execinfo.max_rel_us);
		total_cpu += waveform->modules[i].execinfo.t_exec[0].tv_usec;
	}
	printf("\tTotal\t\t%11d (%.2f%%)\n",total_cpu, (float) 100*total_cpu/tslot_us);
	return 0;
}


void *_run_main(void *arg) {
	int c;
	int tslen;
	hwapi_machine_t machine;
	strcpy(waveform.model_file,arg);
	strcpy(waveform.name,arg);

	hwapi_machine(&machine);
	tslen = machine.ts_len_us;

	hwapi_task_print_sched();

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
		if (c != '\n') {
			printf("\n\nType new command:\n"
					"\t<l>\tLoad waveform\n"
					"\t<i>\tSet INIT\n"
					"\t<r>\tSet RUN\n"
					"\t<p>\tSet PAUSE\n"
					"\t<t>\tSet STEP\n"
					"\t<s>\tStop waveform\n"
					"\t<e>\tView execution time\n"
					"\n<Ctr+C>\tExit\n");
		}
		c = getchar();
		new_status.cur_status = LOADED;
		switch((char) c) {
		case 'l':
			if (waveform_load(&waveform)) {
				aerror("loading waveform\n");
				break;
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
		case 'e':
			if (waveform_update(&waveform)) {
				aerror("updating waveform\n");
				break;
			}
			if (print_execinfo(&waveform,tslen)) {
				aerror("printing execinfo\n");
				break;
			}
			break;
		case '\n':
			break;
		default:
			printf("Unknown command %c\n",(char) c);
			break;
		}
		if (new_status.cur_status != LOADED) {
			new_status.next_timeslot = hwapi_time_slot();
			if (waveform_status_set(&waveform,&new_status)) {
				printf("DID NOT CHANGE!\n");
			} else {
				printf("DONE!\n");
			}
		}
	} while(1);
	/* status init */

	/* pause o sleep o return */

	printf("exit\n");
	return NULL;
}
