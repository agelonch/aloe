#include <assert.h>
#include "metaclass.h"
#include "serializable.h"

void ser_serialize (void *x)
{
	/* use this macro to call the inherited method */
	CALL_INHERITED_METHOD(serializable,serialize,x);
}

void ser_unserialize (void *x)
{
	/* here we left the macro expanded */
	extends *e = find_class(x,serializable_CODE);
	assert(e);
	serializable_methods *ops = (serializable_methods*) e->methods;
	assert(ops);
	ops->unserialize(x);
}
