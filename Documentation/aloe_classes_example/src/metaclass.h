#ifndef METACLASS_H
#define METACLASS_H

/* public method inheritance */
typedef struct _extends {
	int code; /* parent class code */
	void *methods; /* pointer to the the public methods */
	struct _mextend *parent; /* continues parent hierarchy */
} extends;

/* This is the class definition */
typedef struct {
	int code; /* class code */
	int size; /* storage size */
	extends *parent; /* inheritance of public methods */
} class;

/* Assings class to an array of objects */
void init(void *x, class *class, int num_elems);

/* Finds the inherited class with code class_code  */
extends *find_class(void *object, int class_code);


/* MACROS */
/******************************/
#define CLASS(name) name##_class
#define METHODS(name) name##_methods
#define ATTRIBUTES(name) name##_attr
#define CODE(name) name##_CODE


/* assign pointer src to dst casting to class */
#define CAST(dst,src,classname) \
		classname *dst = (classname*) src; \
	assert(dst->class); \
	assert(dst->class->code==classname##_CODE);

#define CALL_INHERITED_METHOD(classname,method,object) \
	extends *__e = find_class(x,classname##_CODE);\
	assert(__e);\
	classname##_methods *__m = (classname##_methods*) __e->methods;\
	assert(__m);\
	__m->method(x);\

#define UNWRAP(...) __VA_ARGS__

/* implements the list of methods (in parenthesis) of interface parentclass */
#define CLASS_EXTEND(class, parentclass,methods) \
	static parentclass##_methods class##parentclass##_methods={UNWRAP methods}; \
	static extends class##parentclass##_extend={parentclass##_CODE,&class##parentclass##_methods,0};

/* if the previous macro was used, define the class with this one */
#define CLASS_DEFINE_INHERITS(name,parent) \
	static class name##_class={name##_CODE,sizeof(name),&name##parent##_extend};

/* define the class with no inheritance */
#define CLASS_DEFINE(name) \
	static class name##_class={name##_CODE,sizeof(name),0};



#endif
