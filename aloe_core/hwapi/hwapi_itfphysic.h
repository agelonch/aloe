#ifndef HWAPI_ITFPHYSIC_H
#define HWAPI_ITFPHYSIC_H

#include "str.h"
#include "hwapi_itf.h"
#include "hwapi.h"


/**
 * In linux, physical interfaces are TCP/IP Ethernet interfaces. They inherit from the Interface abstract class. Supported protocols are TCP and UDP. Modes can be IN, which creates an input socket and accepts the connection in TCP; or OUT, which creates the output socket.
 */

typedef struct {
	hwapi_itf_t parent;
	int sock_fd;
	int conn_fd;
	int is_tcp;
	int connected;
	int id;
}hwapi_itfphysic_t;



#endif
