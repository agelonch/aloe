#include "hwapi_error.h"
#include "hwapi_process.h"
#include "nod_module.h"
#include "str.h"

#define MAX_MODULES 10000

nod_module_t modules[MAX_MODULES];

int dummy_test(char *module_path, int nof_modules, int nof_cores) {
	int k;
	struct hwapi_process_attr attr;
	h_proc_t proc;

	if (!module_path || nof_modules*nof_cores>MAX_MODULES) {
		aerror("Invalid parameters\n");
		return -1;
	}


	ainfo("Creating %d modules and %d pipelines...", nof_modules, nof_cores);
	k=0;
	for (int j=0;j<nof_cores;j++) {
		for (int i=0;i<nof_modules;i++) {
			memset(&attr, 0, sizeof(struct hwapi_process_attr));
			nod_module_t *mod = &modules[k];
			sprintf(mod->parent.name, "module_%d_%d",j,i);
			mod->parent.id = k+1;
			mod->swapi_context.module = mod;
			strncat(attr.binary_path,module_path,LSTR_LEN);
			attr.exec_position = i;
			attr.pipeline_id = j;
			proc = hwapi_process_new(&attr, &modules[k].swapi_context);
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
	ainfo("Done\n");
	return 0;
}
