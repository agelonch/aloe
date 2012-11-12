#ifndef HWAPI_TYPES_H_
#define HWAPI_TYPES_H_

#include <time.h>
#define time_t struct timeval

#include "str.h"
#include "hwapi_types.h"

struct h_proc_ {
	int id;
};
typedef struct h_proc_* h_proc_t;

/**
 * Process attributes for use with the hwapi_process_new() function.
 */
struct hwapi_process_attr {
	strdef(binary_path);
	int pipeline_id;
	int exec_position;
	void (*finish_callback)(h_proc_t);
};



struct h_itf_ {
	int id;
};
typedef struct h_itf_* h_itf_t;


struct h_phyitf_ {
	int id;
};
typedef struct h_phyitf_* h_phyitf_t;

typedef struct {
	int len;
	void *data;
}h_pkt_t;

struct h_qitf_ {
	int id;
};
typedef struct h_qitf_* h_qitf_t;

struct h_dac_ {
	int id;
};
typedef struct h_dac_* h_dac_t;


#endif
