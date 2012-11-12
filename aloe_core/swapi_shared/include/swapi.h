#ifndef SWAPI_H_
#define SWAPI_H_

#include "swapi_types.h"
#include "str.h"

void swapi_exit(void *context);
int swapi_tstamp(void *context);

char *swapi_module_name(void *context);
int swapi_module_id(void *context);

itf_t swapi_itf_create(void *context, string name, swapi_itf_mode_t mode);
int swapi_itf_close(itf_t itf);
int swapi_itf_write(itf_t itf, void* buffer, int size);
int swapi_itf_read(itf_t itf, void* buffer, int size);
int swapi_itf_status(itf_t itf);
void* swapi_itf_ptr_new(itf_t itf, int size);
int swapi_itf_ptr_drop(itf_t itf, void* ptr);
int swapi_itf_ptr_put(itf_t itf, void* ptr);
void* swapi_itf_ptr_get(itf_t itf);

var_t swapi_var_create(void *context, string name, int size, void **ptr);
int swapi_var_initialize(string name, void* ptr, int size);
int swapi_var_close(var_t var);

counter_t swapi_counter_create(void *context, string name);
int swapi_counter_close(counter_t counter);
int swapi_counter_start(counter_t counter);
int swapi_counter_stop(counter_t counter);


log_t swapi_log_create(void *context, string name);
int swapi_log_close(log_t log);
int swapi_log_write(log_t log, string str);
int swapi_log_laerror(log_t log, int va_args);

char* swapi_error_string(void *context);
void swapi_error_print(void *context);
int swapi_error_code(void *context);

#endif
