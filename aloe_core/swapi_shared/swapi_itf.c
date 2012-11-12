#include <stddef.h>
#include "defs.h"
#include "swapi_itf.h"
#include "swapi.h"


/**
 * Creates an logical module interfaces. If the interface is extenal, an object of the class HWAPI.PhysicItf is created, otherwise HWAPI.Queue is created.
 *
 * 1) Find interface name in mymodule.interfaces. The mymodule.interfaces[i].id will be returned as the file descriptor if everything goes ok
 * 2) if mymodule.interface[i].phyItfId>0 the interface is external
 *   2.1) find in hwapi.interfaces an interface with id=phyItfId (do not create, it is already created and connected)
 * 3) if interface is internal
 *   3.1) if mode=WRITE, create an interface of class HWAPI.Queue with address="" and name="%d:%d:%d:%d" <- writerObjId:writerItfId:readerObjId:readerItfId
 *   3.2) if mode=READ, find in hwapi.interfaces an interface with name="%d:%d:%d:%d" <- writerObjId:writerItfId:readerObjId:readerItfId
 * 4) Set interface delay using setDelay()
 * 5) set mymodule.interface[i].active=TRUE
 */
int swapi_itf_init(itf_t itf) {
	aerror("Not yet implemented");
	return -1;
}
/**
 * Closes the interface and frees activeInterfaces. Only internal writer interfaces have to call removeItf
 */
int swapi_itf_close(itf_t itf) {
	aerror("Not yet implemented");
	return -1;
}

/**
 * call activeInterface[fd].send() with same arguments.
 */
int swapi_itf_write(itf_t itf, void* buffer, int size) {
	aerror("Not yet implemented");
	return -1;
}

/**
 * call recv(itf->hwapi_itf) with same arguments.
 */
int swapi_itf_read(itf_t itf, void* buffer, int size) {
	aerror("Not yet implemented");
	return -1;
}

/**
 * returns the number of bytes in the queue
 */
int swapi_itf_status(itf_t itf) {
	aerror("Not yet implemented");
	return -1;
}

/**
 * call activeInterface[fd].requestPtr() with same arguments.
 */
void* swapi_itf_ptr_new(itf_t itf, int size) {
	aerror("Not yet implemented");
	return NULL;
}

/**
 * call activeInterface[fd].releasePtr() with same arguments.
 */
int swapi_itf_ptr_drop(itf_t itf, void* ptr) {
	aerror("Not yet implemented");
	return -1;
}

/**
 * call activeInterface[fd].putPtr() with same arguments.
 */
int swapi_itf_ptr_put(itf_t itf, void* ptr) {
	aerror("Not yet implemented");
	return -1;
}

/**
 * call activeInterface[fd].getPtr() with same arguments.
 */
void* swapi_itf_ptr_get(itf_t itf) {
	aerror("Not yet implemented");
	return NULL;
}
