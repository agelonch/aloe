/* 
 * Copyright (c) 2012, Ismael Gomez-Miguelez <ismael.gomez@tsc.upc.edu>.
 * This file is part of ALOE++ (http://flexnets.upc.edu/)
 * 
 * ALOE++ is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * ALOE++ is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with ALOE++.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <pthread.h>
#include "defs.h"
#include "hwapi.h"
#include "nod_waveform.h"
#include "swapi_static.h"
#include "swapi_context.h"

int call_init(void *arg);
int call_stop(void *arg);
void* _call_stop(void *arg);

int _run_cycle(void* context) {
	swapi_context_t *ctx = (swapi_context_t*) context;
	nod_module_t *module = (nod_module_t*) ctx->module;
	nod_waveform_t *waveform = (nod_waveform_t*) module->parent.waveform;
	sdebug("context=0x%x, module_id=%d, changing_status=%d, cur_status=%d waveform_status=%d\n",context,
			module->parent.id, module->changing_status, module->parent.status, waveform->status.cur_status);

	if (waveform->status.cur_status == LOADED && module->parent.status == PARSED) {
		module->parent.status = LOADED;
	}

	/* Change only if finished previous status change */
	if (!module->changing_status && module->parent.status != waveform->status.cur_status) {
		sdebug("next_tslot=%d, cur_tslot=%d\n",waveform->status.next_timeslot, hwapi_time_slot());
		/* is it time to change? */
		if (hwapi_time_slot() >= waveform->status.next_timeslot) {
			switch(waveform->status.cur_status) {
			case INIT:
				module->changing_status = 1;
				if (call_init(ctx)) {
					aerror("calling init\n");
				}
			break;
			case STOP:
				hwapi_process_seterror(module->process,FINISH_OK);
				module->changing_status = 1;
				if (call_stop(ctx)) {
					aerror("calling stop\n");
				}
			break;
			case PAUSE:
			case RUN:
				/* These status does not need confirmation */
				module->parent.status = waveform->status.cur_status;
			break;
			default:
				break;
			}
		}
	}

	if (!module->changing_status && waveform->status.cur_status == RUN) {
		/* save start time */
		hwapi_time_get(&module->parent.execinfo.t_exec[1]);

		/* run aloe cycle */
		if (Run(context)) {
			sdebug("RUNERROR: module_id=%d\n",module->parent.id);

			/* set run-time error code */
			if (hwapi_process_seterror(module->process,RUNERROR)) {
				aerror("hwapi_process_seterror");
			}

			/* and stop thread. hwapi will notify finish_callback() process function */
			if (hwapi_process_stop(module->process)) {
				aerror("hwapi_process_stop");
			}
		}

		/* save end time */
		hwapi_time_get(&module->parent.execinfo.t_exec[2]);

		/* compute execution time, exponential average, max, etc. and save data to mymodule.execinfo */
#ifdef kk
		/* stat reports */
		for (i=0;i<nof_reporting_vars;i++) {
			module.reporting_variables[i].period_cnt++;
			/* save the first window samples only */
			if (module.reporting_variables[i].period_cnt++<module.reporting_variables[i].window)
				module.reporting_variables[i].serialize(module.reporting_variables[i].report_packet);
			}
			/* send report every period */
			if (module.reporting_variables[i].periodCnt++<module.reporting_variables[i].period) {
				hwapi.newTask(report_variable,&reporting_variables[i]);
			}
		}

		/* write logs */
		for (i=0;i<nof_logs;i++) {
			if (logs[i].w_ptr) {
				hwapi.new_task(swapi_log._writelog,logs[i]);
			}
		}
#endif
	}
	return 0;
}


void* _call_init(void *arg) {
	int n;
	nod_module_t *module = swapi_get_module(arg);
	nod_waveform_t *waveform = module->parent.waveform;
	sdebug("module_id=%d, changing_status=%d, now_is=0\n",module->parent.id,
			module->changing_status);

	while((n=Init(arg)) == 0) {
		hwapi_sleep(1);
	}
	sdebug("module_id=%d finished init. Setting status to %d\n",module->parent.id,
			waveform->status.cur_status);
	if (n > 0) {
		module->changing_status = 0;
		module->parent.status = waveform->status.cur_status;
	}
	module->status_task = 0;
	return NULL;
}

int call_init(void *arg) {
	nod_module_t *module = swapi_get_module(arg);
	return hwapi_task_new(&module->status_task,_call_init,arg);
}

void* _call_stop(void *arg) {
	nod_module_t *module = swapi_get_module(arg);
	sdebug("module_id=%d, changing_status=%d, now_is=0\n",module->parent.id,
			module->changing_status);
	nod_waveform_t *waveform = module->parent.waveform;

	if (Stop(arg)) {
		module->status_task = 0;
		return NULL;
	}
	sdebug("module_id=%d finished stop. Setting status to %d\n",module->parent.id,
			waveform->status.cur_status);

	module->changing_status = 0;
	module->parent.status = waveform->status.cur_status;
	module->status_task = 0;
	return NULL;
}


int call_stop(void *arg) {
	nod_module_t *module = swapi_get_module(arg);
		return hwapi_task_new(&module->status_task,_call_stop,arg);
}

