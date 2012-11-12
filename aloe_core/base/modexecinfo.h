#ifndef EXECINFO_H
#define EXECINFO_H

#include "time.h"
#include "manapi_types.h"
#include "packet.h"

/* Inherited methods */
int execinfo_update(execinfo_t *obj);
void execinfo_serialize(execinfo_t *src, packet_t *pkt);
void execinfo_unserializeTo(packet_t *pkt, execinfo_t *dest);

#endif

