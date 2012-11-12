#ifndef HWAPI_ITF_H
#define HWAPI_ITF_H

#include "str.h"
#include "hwapi.h"

/**
 * Abstract class that manages the hwapi data or control, physical or logical interfaces.
 */
typedef struct {
	int id;

	strdef(name);
	int mode_is_input;
	/**
	 * Interface delay, in timeslots
	 */
	int delay;
	lstrdef(address);
	int is_blocking;
	void (*callback)(void);

}hwapi_itf_t;

#endif
