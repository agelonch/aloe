#ifndef MAN_PROBELISTENER_H
#define MAN_PROBELISTENER_H

#include "objects_max.h"
#include "hwapi.h"
#include "man_probehandlers.h"

typedef struct {
	man_probehandlers_t handlers[MAX(man_probehandlers)];
	h_itf_t* itf;
}man_probelistener_t;

int man_probelistener_add(man_probelistener_t *lstnr, void (*callback)(void), int code);
int man_probelistener_remove(man_probelistener_t *lstnr, int code);
int man_probelistener_setup(man_probelistener_t *lstnr, h_itf_t *itf);
int man_probelistener_process(man_probelistener_t *lstnr);

#endif
