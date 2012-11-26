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

#include <stddef.h>
#include "hwapi.h"
#include "hwapi_process.h"
#include "nod_waveform.h"
#include "str.h"

#define MAX_MODULES 10000

nod_module_t modules[MAX_MODULES];

int dummy_test(char *module_path, int nof_modules, int nof_cores) {
	int k;
	struct hwapi_process_attr attr;
	h_proc_t proc;

	if (!module_path || nof_modules*nof_cores>MAX_MODULES || nof_modules < 0) {
		aerror("Invalid parameters\n");
		return -1;
	}


	ainfo("Creating %d modules and %d pipelines.\n", nof_modules, nof_cores);
	k=0;
	for (int j=0;j<nof_cores;j++) {
		for (int i=0;i<nof_modules;i++) {
			memset(&attr, 0, sizeof(struct hwapi_process_attr));
			nod_module_t *mod = &modules[k];
			sprintf(mod->parent.name, "module_%d_%d",j,i);
			mod->parent.id = k+1;
			strncat(attr.binary_path,module_path,LSTR_LEN);
			attr.exec_position = i;
			attr.pipeline_id = j;
			proc = hwapi_process_new(&attr, &modules[k].context);
			if (proc == NULL) {
				hwapi_perror("hwapi_process_new");
				return -1;
			}
			if (hwapi_process_run(proc)) {
				hwapi_perror("hwapi_process_run");
				return -1;
			}
			k++;
		}
	}
	return 0;
}
