
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "metaclass.h"
#include "module.h"
#include "man_module.h"

module modules[2];
man_module man_modules[2];

int main(int argc, char **argv)
{
	module *tmp;

	/* first assign the class to statically allocated objects.
	 * This needs to be done at run-time */
	init(modules,&CLASS(module),2);
	init(man_modules,&CLASS(man_module),2);

	strcpy(modules[0].name,"bob");
	strcpy(modules[1].name,"alice");

	strcpy(man_modules[0].parent.name,"m_bob");
	strcpy(man_modules[1].parent.name,"m_alice");

	/* man_modules can be casted to module (since it inherits attributes) */
	tmp=(module*) &man_modules[1];

	printf("and access his members %s\n",tmp->name);

	/* Inherited methods (which are implemented differently for each class) are called
	 * using the same interface. */
	ser_serialize(&modules[0]);
	ser_serialize(&man_modules[0]);

	printf("bye\n");

	return 1;
}

/* The End. */
