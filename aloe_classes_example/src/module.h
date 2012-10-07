#ifndef MODULE_H
#define MODULE_H

#include "metaclass.h"
#include "serializable.h"
#include "str.h"

#define module_CODE 0x1234abcf

/* Class definition (all attributes are public) */
typedef struct {
	class *class;
	int id;
	str(name);
	str(binary);
	int status;
} module;

/* define public attributes that can be inherited
 * (MUST FOLLOW the same order than in module class structure) */
typedef struct {
	int id;
	str(name);
	str(binary);
	int status;
} ATTRIBUTES(module);

/* inherited methods */
void module_serialize(void*);
void module_unserialize(void*);

/* this class extends serializable methods */
CLASS_EXTEND(module,serializable,(module_serialize,module_unserialize));

/* define class as extension of serializable */
CLASS_DEFINE_INHERITS(module,serializable);

#endif
