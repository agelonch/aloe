/* 
 * Copyright (c) 2012, Ismael Gomez-Miguelez <ismael.gomez@tsc.upc.edu>.
 * This file is part of ALOE++ (http://flexnets.upc.edu/)
 * 
 * ALOE++ is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * ALOE++ is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with ALOE++.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef HWAPI_H_
#define HWAPI_H_

#define MODULES_PATH "/home/ismael/aloe_ws/aloe_git/modules"

#include "str.h"
#include "hwapi_types.h"
#include "hwapi_machine.h"
#include "swapi_types.h"

#define BINARIES_BASE_DIR "../modules/"
#define BINARIES_MODULE_DIR "../lib/"


typedef enum {
	FINISH_OK, RTFAULT, SIG_RECV, RUNERROR
}hwapi_processerrors_t;

void hwapi_machine(hwapi_machine_t *machine);

int hwapi_start_manager_interfaces(string config_file);

int hwapi_sleep_to(int wake_tslot);
int hwapi_sleep(int nof_slots);

int hwapi_periodic_add(void (*fnc)(void), int period);
int hwapi_periodic_remove(void (*fnc)(void));

char* hwapi_error_string();
int hwapi_error_code();
void hwapi_error_print(const char *user_message);

h_proc_t hwapi_process_new(struct hwapi_process_attr *attr, void *arg);
int hwapi_process_remove(h_proc_t proc);
int hwapi_process_pid(h_proc_t proc);
int hwapi_process_run(h_proc_t proc);
int hwapi_process_stop(h_proc_t process);
int hwapi_process_seterror(h_proc_t proc, hwapi_processerrors_t code);
hwapi_processerrors_t hwapi_process_geterror(h_proc_t proc);

int hwapi_task_new(h_task_t *task, void *(*fnc)(void*), void* arg);
int hwapi_task_new_prio(h_task_t *task, void *(*fnc)(void*), void* arg, int prio, int cpu);
int hwapi_task_kill(h_task_t task);
int hwapi_task_wait(h_task_t task, void **retval);

int hwapi_time_set(time_t *time);
int hwapi_time_get(time_t *time);
int hwapi_time_slot();
void hwapi_time_interval(time_t * tdata);
int hwapi_time_attach_local();

int hwapi_file_open(string name);
int hwapi_file_close(int fd);
int hwapi_file_write(int fd, void* buf, int size);
int hwapi_file_read(int fd, void* buf, int size);

h_itf_t hwapi_itfphysic_get(string name);
h_itf_t hwapi_itfphysic_get_id(int id);
int hwapi_itfphysic_create(h_itf_t obj, string address);
int hwapi_itfphysic_connect(h_itf_t obj);
int hwapi_itfphysic_disconnect(h_itf_t obj);

h_itf_t hwapi_itfqueue_new(int max_msg, int msg_sz);
int hwapi_itf_remove(h_itf_t obj);
int hwapi_itf_send(h_itf_t obj, void* buffer, int len);
int hwapi_itf_recv(h_itf_t obj, void* buffer, int len);
int hwapi_itf_set_callback(h_itf_t obj, void (*fnc)(void), int prio);
int hwapi_itf_set_blocking(h_itf_t obj, int block);
int hwapi_itf_get_blocking(h_itf_t obj);
h_pkt_t* hwapi_itf_request_pkt(h_itf_t obj);
int hwapi_itf_put_pkt(h_itf_t obj, h_pkt_t* ptr);
h_pkt_t* hwapi_itf_get_pkt(h_itf_t obj);
int hwapi_itf_release_pkt(h_itf_t obj, h_pkt_t* ptr);
int hwapi_itf_set_delay(h_itf_t obj, int delay);
int hwapi_itf_get_delay(h_itf_t obj);


h_dac_t hwapi_dac_get(string address);
int hwapi_dac_set_scheduler(h_dac_t obj, void (*ts_begin_fnc)(void), int thread_prio);
int hwapi_dac_start(h_dac_t obj);
int hwapi_dac_set_opts(h_dac_t obj, string opts);
int hwapi_dac_set_freq(h_dac_t obj, float freq);
int hwapi_dac_set_block_len(h_dac_t obj, int len);
int hwapi_dac_set_sample_type(h_dac_t obj, int type);
int hwapi_dac_set_buffer_size(h_dac_t obj, int in, int out);
h_itf_t hwapi_dac_channel(h_dac_t obj, int int_ch);


#define PROC_ERRCODE_OK 	1
#define PROC_ERRCODE_RUN 	2


/* Called from outside API. Prints hwapi error in one line and current file/line plus message in another */
#define hwapi_perror(msg) hwapi_error_print(""); aerror(msg)


#endif
