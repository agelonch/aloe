#include <assert.h>
#include <stdio.h>
#include "module.h"
#include "man_module.h"
#include "metaclass.h"
#include "serializable.h"

void man_module_serialize(void *self)
{
	CAST(m,self,man_module);

	/* implement here overloaded method */
	printf("man module %s is serializing\n", m->parent.name);

}

void man_module_unserialize(void *self)
{
	CAST(m,self,man_module);

	/* implement here overloaded method */
	printf("man module %s is unserializing\n", m->parent.name);
}

