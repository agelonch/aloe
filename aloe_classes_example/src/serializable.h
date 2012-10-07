#ifndef SERIALIZABLE_H
#define SERIALIZABLE_H

#include "metaclass.h"

#define serializable_CODE	0xabcd1234

/** public methods that will be implemented by the inherited class.
 */
typedef struct {
	void (*serialize) (void*);
	void (*unserialize) (void*);
} METHODS(serializable);


void ser_serialize (void*);
void ser_unserialize (void*);

#endif
