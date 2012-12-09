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

#ifndef rtdal_H_
#define rtdal_H_

#define MODULES_PATH "/home/ismael/aloe_ws/aloe_git/modules"

#include "rtdal_types.h"
#include "str.h"
#include "rtdal_machine.h"

#define BINARIES_BASE_DIR "../modules/"
#define BINARIES_MODULE_DIR "../lib/"

/**@defgroup runmain _run_main() function
 * @{
 */
void *_run_main(void *arg);
/**@} */

/**@defgroup other Other functions
 * @{
 */
void rtdal_machine(rtdal_machine_t *machine);
int rtdal_sleep(time_t *t);
/**@} */


/** @defgroup process Pipeline processes functions
 * @{
 */

typedef enum {
	FINISH_OK, 	/**< The process is running or finished successfully */
	RTFAULT, 	/**< The process missed the defined real-time deadline */
	SIG_RECV, 	/**< The process received a thread-specific signal (SIGSEGV, SIGILL, SIGBUF or SIGFPE) */
	RUNERROR	/**< An error occured during process execution.
	This error is set manually using rtdal_process_seterror() */
}rtdal_processerrors_t;


r_proc_t rtdal_process_new(struct rtdal_process_attr *attr, void *arg);
int rtdal_process_remove(r_proc_t proc);
int rtdal_process_run(r_proc_t proc);
int rtdal_process_stop(r_proc_t process);
int rtdal_process_seterror(r_proc_t proc, rtdal_processerrors_t code);
rtdal_processerrors_t rtdal_process_geterror(r_proc_t proc);
int rtdal_process_isrunning(r_proc_t proc);

/**@}
 */

/**@defgroup task Low-priority task functions
 * @{ */
int rtdal_task_new(r_task_t *task, void *(*fnc)(void*), void* arg);
int rtdal_task_kill(r_task_t task);
int rtdal_task_wait(r_task_t task, void **retval);
int rtdal_task_wait_nb(r_task_t task, void **retval);
void rtdal_task_print_sched();
/**@} */

/**@defgroup period Periodic low-priority tasks functions
 *@{
 */
int rtdal_periodic_add(void (*fnc)(void), int period);
int rtdal_periodic_remove(void (*fnc)(void));
/**@} */

/**@defgroup time Time functions
 * @{
 */
int rtdal_time_set(time_t *time);
int rtdal_time_get(time_t *time);
int rtdal_time_slot();
void rtdal_time_interval(time_t * tdata);
int rtdal_time_attach_local();
/**@} */

/**@defgroup file Filesystem I/O functions
 * @{
 */
int rtdal_file_open(string name);
int rtdal_file_close(int fd);
int rtdal_file_write(int fd, void* buf, int size);
int rtdal_file_read(int fd, void* buf, int size);
/**@} */

/**@defgroup itf Interfaces functions
 * @{
 */
r_itf_t rtdal_itfphysic_get(string name);
r_itf_t rtdal_itfphysic_get_id(int id);
int rtdal_itfphysic_create(r_itf_t obj, string address);
int rtdal_itfphysic_connect(r_itf_t obj);
int rtdal_itfphysic_disconnect(r_itf_t obj);

r_itf_t rtdal_itfspscq_new(int max_msg, int msg_sz);

int rtdal_itf_remove(r_itf_t obj);
int rtdal_itf_send(r_itf_t obj, void* buffer, int len);
int rtdal_itf_recv(r_itf_t obj, void* buffer, int len);
int rtdal_itf_set_callback(r_itf_t obj, void (*fnc)(void), int prio);
int rtdal_itf_set_blocking(r_itf_t obj, int block);
int rtdal_itf_get_blocking(r_itf_t obj);
int rtdal_itf_push(r_itf_t obj, int len);
int rtdal_itf_pop(r_itf_t obj, void **ptr, int *len);
int rtdal_itf_request(r_itf_t obj, void **ptr);
int rtdal_itf_release(r_itf_t obj);
int rtdal_itf_send(r_itf_t obj, void* buffer, int len);
int rtdal_itf_recv(r_itf_t obj, void* buffer, int len);
int rtdal_itf_set_delay(r_itf_t obj, int delay);
int rtdal_itf_get_delay(r_itf_t obj);
/**@} */

/**@defgroup dac AD/DA functions
 * @{
 */
r_dac_t rtdal_dac_get(string address);
int rtdal_dac_set_scheduler(r_dac_t obj, void (*ts_begin_fnc)(void), int thread_prio);
int rtdal_dac_start(r_dac_t obj);
int rtdal_dac_set_opts(r_dac_t obj, string opts);
int rtdal_dac_set_freq(r_dac_t obj, float freq);
int rtdal_dac_set_block_len(r_dac_t obj, int len);
int rtdal_dac_set_sample_type(r_dac_t obj, int type);
int rtdal_dac_set_buffer_size(r_dac_t obj, int in, int out);
r_itf_t rtdal_dac_channel(r_dac_t obj, int int_ch);
/**@} */


/** @defgroup error Error functions
 * @{
 */
char* rtdal_error_string();
int rtdal_error_code();
void rtdal_error_print(const char *user_message);
#define rtdal_perror(msg) rtdal_error_print(""); aerror(msg)
/** @} */


/** \mainpage ALOE++ Real-Time Distributed Abstraction Layer (RTDAL)
 *
 * RTDAL is a framework for distributed real-time processing.
 */



#endif
