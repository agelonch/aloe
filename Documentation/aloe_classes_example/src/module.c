#include <stdio.h>
#include <assert.h>
#include "module.h"
#include "metaclass.h"
#include "serializable.h"


void module_serialize(void *self)
{
	CAST(m,self,module);

	/* implement here overloaded method */
	printf("module %s is serializing\n", m->name);
}


void module_unserialize(void *self)
{
	CAST(m,self,module);

	/* implement here overloaded method */
	printf("module %s is unserializing\n", m->name);
}

