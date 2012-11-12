#ifndef MANAPI_H_
#define MANAPI_H_

#include "manapi_types.h"


int manapi_initialize(string platform_conf, string waveforms_repository);
int manapi_read_waveforms(string repository);
int manapi_start_nodes();
int manapi_stop_nodes();
waveform_t* manapi_waveform_get(string name);

int manapi_error_code();
char* manapi_error_string();
void manapi_error_print();

int update(updateable *obj);

int man_platform_update();
int man_platform_connect();
int man_platform_disconnect();
int man_platform_tslot_length();
man_platform_model_t* man_platform_get_model();

int variable_report_start(variable_t *var, void (*callback)(void), int period, int window);
int variable_report_stop(variable_t *var);

int waveform_load(waveform_t* waveform);
int waveform_parse(waveform_t* waveform);

int waveform_status_to_string(waveform_t* waveform);
int waveform_status_set(waveform_status_t* status, waveform_status_enum new_status, int next_ts, int dead_ts);
int waveform_status_is_loaded(waveform_status_t* status);
int waveform_status_is_running(waveform_status_t* status);


#endif
