#ifndef HWAPI_PERIODIC_H
#define HWAPI_PERIODIC_H

typedef struct {
	void (*callback)(void);
	int period;
	int counter;
}hwapi_periodic_t;

#endif
