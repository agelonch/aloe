#include <stddef.h>
#include "defs.h"
#include "str.h"
#include "hwapi_itf.h"
#include "hwapi_itfphysic.h"
#include "hwapi.h"

int hwapi_itfphysic_create(h_itf_t obj, string address) {
	aerror("Not yet implemented");
	return -1;
}

int hwapi_itfphysic_connect(h_phyitf_t obj) {
	aerror("Not yet implemented");
	return -1;
}

int hwapi_itfphysic_disconnect(h_phyitf_t obj) {
	aerror("Not yet implemented");
	return -1;
}

int hwapi_itfphysic_remove(h_itf_t obj) {
	aerror("Not yet implemented");
	return -1;
}

int hwapi_itfphysic_recv(h_itf_t obj, void* buffer, int len) {
	aerror("Not yet implemented");
	return -1;
}

int hwapi_itfphysic_send(h_itf_t obj, void* buffer, int len) {
	aerror("Not yet implemented");
	return -1;
}

int hwapi_itfphysic_set_blocking(h_itf_t obj, int block) {
	aerror("Not yet implemented");
	return -1;
}

int hwapi_itfphysic_get_blocking(h_itf_t obj) {
	aerror("Not yet implemented");
	return -1;
}

void* hwapi_itfphysic_request_ptr(h_itf_t obj, int size) {
	aerror("Not yet implemented");
	return NULL;
}

int hwapi_itfphysic_release_ptr(h_itf_t obj, void* ptr) {
	aerror("Not yet implemented");
	return -1;
}

int hwapi_itfphysic_put_ptr(h_itf_t obj, void* ptr) {
	aerror("Not yet implemented");
	return -1;
}

int hwapi_itfphysic_set_callback(h_itf_t obj, void (*fnc)(void), int prio) {
	aerror("Not yet implemented");
	return -1;
}

int hwapi_itfphysic_set_delay(h_itf_t obj, int delay) {
	aerror("Not yet implemented");
	return -1;
}

int hwapi_itfphysic_get_delay(h_itf_t obj) {
	return -1;//hwapi_itf_get_delay__(obj);
}

void* hwapi_itfphysic_get_ptr(h_itf_t obj) {
	aerror("Not yet implemented");
	return NULL;
}
