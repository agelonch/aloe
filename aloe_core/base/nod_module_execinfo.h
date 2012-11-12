#ifndef NOD_MODULE_EXECINFO_H
#define NOD_MODULE_EXECINFO_H

#include "modexecinfo.h"
#include "packet.h"

/**
 * The class attributes save information relative to the execution of the module.
 */
typedef struct  {
	execinfo_t parent;
} nod_module_execinfo_t;

int add_sample(nod_module_execinfo_t *execinfo, int cpu, int relinquish);
void nod_module_execinfo_serialize(nod_module_execinfo_t *dest, packet_t *pkt);
void nod_module_execinfo_unserializeTo(packet_t *pkt, nod_module_execinfo_t *dest);

#endif
