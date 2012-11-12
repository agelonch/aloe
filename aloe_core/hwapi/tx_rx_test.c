#include "hwapi_error.h"
#include "hwapi_process.h"
#include "nod_module.h"
#include "str.h"

nod_module_t modules[2];

int tx_rx_test(char *tx_path, char *rx_path) {
	struct hwapi_process_attr attr;
	h_proc_t proc;

	if (!tx_path || !rx_path) {
		aerror("Invalid parameters\n");
		return -1;
	}

	ainfo("Creating tx... ");
	memset(&attr, 0, sizeof(struct hwapi_process_attr));
	nod_module_t *mod = &modules[0];
	strcat(mod->parent.name, "tx");
	mod->parent.id = 1;
	mod->swapi_context.module = mod;
	strncat(attr.binary_path,tx_path,LSTR_LEN);
	attr.exec_position = 1;
	attr.pipeline_id = 0;
	proc = hwapi_process_new(&attr, &modules[0].swapi_context);
	if (proc == NULL) {
		hwapi_perror("hwapi_process_new");
		return -1;
	}
	if (hwapi_process_run(proc)) {
		hwapi_perror("hwapi_process_run");
		return -1;
	}

	ainfo("Creating rx...");
	memset(&attr, 0, sizeof(struct hwapi_process_attr));
	mod = &modules[1];
	strcat(mod->parent.name, "rx");
	mod->parent.id = 2;
	mod->swapi_context.module = mod;
	strncat(attr.binary_path,rx_path,LSTR_LEN);
	attr.exec_position = 2;
	attr.pipeline_id = 0;
	proc = hwapi_process_new(&attr, &modules[1].swapi_context);
	if (proc == NULL) {
		hwapi_perror("hwapi_process_new");
		return -1;
	}
	if (hwapi_process_run(proc)) {
		hwapi_perror("hwapi_process_run");
		return -1;
	}

	ainfo("Done\n");
	return 0;
}
