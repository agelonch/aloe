#ifndef MANMODULE_H
#define MANMODULE_H

#include "metaclass.h"
#include "serializable.h"
#include "str.h"
#include "module.h"

#define man_module_CODE 0x1134abcf

/* Class definition (all attributes are public */
typedef struct {
	class *class;
	/* only the attributes from module are inherited. Hence just place here the structure
	 */
	ATTRIBUTES(module) parent;

	/* public methods of the man_module class go here */
	int another_public_method;
} man_module;

/* public methods */
void update(man_module *self);

/* inherited methods  */
void man_module_serialize(void*);
void man_module_unserialize(void*);

CLASS_EXTEND(man_module,serializable,(man_module_serialize,man_module_unserialize));
CLASS_DEFINE_INHERITS(man_module,serializable);

#endif
