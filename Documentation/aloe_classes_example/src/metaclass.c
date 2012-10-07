#include <stdio.h>
#include <assert.h>
#include "metaclass.h"

extends *find_class(void *object, int class_code)
{
	class **c = (class**) object;
	extends *e;

	/* make sure the object is part of a class */
	assert((*c)->code);

	while((e=(*c)->parent)) {
		if (e->code==class_code)
			return e;
	}
	return NULL;
}

void init(void *x, class *x_class, int num_elems)
{
	int i;
	class **p=(class**) x_class;
	for (i=0;i<num_elems;i++) {
		*p = x_class;
		p+=x_class->size/sizeof(p);
	}
}
