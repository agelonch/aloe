#ifndef MAN_PROBEHANDLERS_H
#define MAN_PROBEHANDLERS_H

typedef struct {

	int code;
	void (*callback)(void);
}man_probehandlers_t;

#endif
