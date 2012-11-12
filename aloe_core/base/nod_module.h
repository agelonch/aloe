#ifndef NOD_MODULE_H
#define NOD_MODULE_H

#include "nod_module_execinfo.h"
#include "nod_variable.h"
#include "interface.h"
#include "objects_max.h"
#include "hwapi.h"
#include "swapi_context.h"
#include "module.h"

/**
 * Stores the attributes of waveform module.
 */
typedef struct {
	module_t parent;
	nod_module_execinfo_t execinfo;
	swapi_context_t swapi_context;
	h_proc_t *process;
	interface_t interfaces[MAX(nod_interfaces)];
	nod_variable_t variables[MAX(nod_variables)];
} nod_module_t;

void nod_module_serialize(nod_module_t *src, packet_t *pkt);
void nod_module_unserializeTo(packet_t *pkt, nod_module_t *dest);

#endif
