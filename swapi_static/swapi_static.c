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

#include "defs.h"
#include "hwapi.h"
#include "nod_waveform.h"
#include "swapi_static.h"
#include "swapi_context.h"


int _run_cycle(void* context) {
	swapi_context_t *ctx = (swapi_context_t*) context;
	nod_module_t *module = (nod_module_t*) ctx->module;
	nod_waveform_t *waveform = (nod_waveform_t*) module->parent.waveform;
	sdebug("context=0x%x, module_id=%d, changing_status=%d, cur_status=%d waveform_status=%d\n",context,
			module->parent.id, module->changing_status, module->parent.status, waveform->status.cur_status);

	/* Change only if finished previous status change */
	if (!module->changing_status && module->parent.status != waveform->status.cur_status) {
		sdebug("next_tslot=%d, cur_tslot=%d\n",waveform->status.next_timeslot, hwapi_time_slot());
		/* is it time to change? */
		if (hwapi_time_slot() >= waveform->status.next_timeslot) {
			switch(waveform->status.cur_status) {
			case INIT:
				module->changing_status = 1;
				call_status(Init, ctx,waveform->status.cur_status);
			break;
			case STOP:
				hwapi_process_seterror(module->process,FINISH_OK);
				module->changing_status = 1;
				call_status(Stop, ctx,waveform->status.cur_status);
			break;
			case PAUSE:
			case RUN:
				/* These status does not need confirmation */
				module->changing_status = 0;
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
		if (!Run(context)) {
			/* set run-time error code */
			hwapi_process_seterror(module->process,RUNERROR);

			/* abort execution */
			call_status(Stop, ctx,STOP);
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

void _abort(void* context) {
	aerror("Not yet implemented");
}
