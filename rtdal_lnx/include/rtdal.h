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


/**@defgroup runmain _run_main() function
 *
 * This function is implemented in an external file by the user. This file must be linked
 * with the RTDAL executable at compile time.
 *
 * When this function returns, the RTDAL program the execution.
 * @{ */

/**
 * The _run_main() function is called by the RTDAL at boot. The return value is ignored.
 */
void *_run_main(void *arg);
/**@} */

/**@defgroup other Other functions
 * @{ */
void rtdal_machine(rtdal_machine_t *machine);
int rtdal_sleep(time_t *t);
/**@} */


/** @defgroup process Synchronous High-Priority Tasks
 * @{ */

/** RTDAL process error code. */
typedef enum {
	FINISH_OK, 	/**< The process is running or finished successfully */
	RTFAULT, 	/**< The process missed the defined real-time deadline */
	SIG_RECV, 	/**< The process received a thread-specific signal (SIGSEGV, SIGILL, SIGBUF or SIGFPE) */
	RUNERROR,	/**< An error occured during process execution.
				This error is set manually using rtdal_process_seterror() */
	USR1,		/**< Custom user signal, set manually using rtdal_process_seterror() */
	USR2		/**< Custom user signal, set manually using rtdal_process_seterror() */
}rtdal_processerrors_t;

/** Process attributes for use with the rtdal_process_new() function. */
struct rtdal_process_attr {
	strdef(binary_path); /**< Path relative to /usr/local/lib to the shared library where the program code resides*/
	int pipeline_id; /** The pipeline_id is the index of the processing core where the process
					should be allocated */
	int exec_position; /**< Indicates the execution position in the processing core. */
	int process_group_id;
	void* (*finish_callback)(void*); /**< Callback function used to notify changes in rtdal_processerror_t */
};

/** Maximum process_group_id number */
#define MAX_PROCESS_GROUP_ID	100

r_proc_t rtdal_process_new(struct rtdal_process_attr *attr, void *arg);
int rtdal_process_remove(r_proc_t proc);
int rtdal_process_run(r_proc_t proc);
int rtdal_process_stop(r_proc_t process);
int rtdal_process_seterror(r_proc_t proc, rtdal_processerrors_t code);
rtdal_processerrors_t rtdal_process_geterror(r_proc_t proc);
int rtdal_process_isrunning(r_proc_t proc);
int rtdal_process_group_notified(r_proc_t proc);

/**@} */

/** @defgroup task Asynchronous Low-Priority Tasks
 * @{ */
int rtdal_task_new(r_task_t *task, void *(*fnc)(void*), void* arg);
int rtdal_task_kill(r_task_t task);
int rtdal_task_wait(r_task_t task, void **retval);
int rtdal_task_wait_nb(r_task_t task, void **retval);
void rtdal_task_print_sched();
/**@} */

/**@defgroup period Synchronous Low-Priority Tasks
 * @{ */
int rtdal_periodic_add(void (*fnc)(void), int period);
int rtdal_periodic_remove(void (*fnc)(void));
/**@} */

/**@defgroup time Time functions
 * These set of functions are used to obtain or modify the system time.
 * @{
 */
int rtdal_time_set(time_t *time);
int rtdal_time_get(time_t *time);
int rtdal_time_slot();
void rtdal_time_interval(time_t * tdata);
int rtdal_time_attach_local();
/**@} */

/**@defgroup file Filesystem I/O functions
 * TODO: documentent this
 * @{
 */
int rtdal_file_open(string name);
int rtdal_file_close(int fd);
int rtdal_file_write(int fd, void* buf, int size);
int rtdal_file_read(int fd, void* buf, int size);
/**@} */

/**@defgroup itf Interfaces functions
 * @{ */

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

/**@defgroup dac AD/DA interface
 * TODO: documentent this
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
 *
 * The error functions are used to print the errors produced during a call to an RTDAL function.
 *
 * When a function returns -1, the error is stored in a variable which is obtained using
 * rtdal_error_code() and equals one of the RTDAL_ERROR_* constants.
 *
 * An error message can be obtained using the functions rtdal_error_string(), rtdal_error_print()
 * or the macro rtdal_perror().
 *
 * @FIXME: This part is not thread-safe.
 *
 * @{
 */

#define RTDAL_ERROR_INVAL 		1
#define RTDAL_ERROR_SYSTEM 		2
#define RTDAL_ERROR_OTHER 		3
#define RTDAL_ERROR_NOSPACE 	4
#define RTDAL_ERROR_LARGE 		5
#define RTDAL_ERROR_NOTFOUND 	6
#define RTDAL_ERROR_DL			7


char* rtdal_error_string();
int rtdal_error_code();
void rtdal_error_print(const char *user_message);
#define rtdal_perror(msg) rtdal_error_print(""); aerror(msg)
/** @} */


/** \mainpage ALOE++ Real-Time Distributed Abstraction Layer (RTDAL)
 *
 * RTDAL facilitates real-time synchronous execution of tasks in a distributed environment.
 * Tasks are executed periodically on each processor in a pipeline fashion.
 * Each processor creates one thread per core, which runs each task (dynamically loaded as a
 * shared library) one after another. The threads period on each core of each processor are
 * continuously synchronized, offering the user an abstracted virtual platform.
 * It is also possible to synchronize the task execution with a digital converter (AD/DA) for
 * coherent transmission and processing of samples.
 *
 * Besides, the RTDAL API also provides other functions to abstract the specific platform-dependant
 * characteristics:
 *    * Low-priority tasks: Synchronous or asynchronous low-priority tasks can be created and managed.
 *    * Interfaces: Two tasks sharing a common interface can communicate between each other. Interfaces can be external or internal, to communicate tasks in remote or local processors, respectively. Internal interfaces support zero-copy mode where only a pointer is transfered between the writer and reader, minimizing memory bandwidth consumption. The current implementation employs a wait-free SPSC bounded queue for best real-time performance.
 *    * AD/DA abstraction, time functions, shared memory, file I/O, etc.
 *
 *
 * When compiled, RTDAL creates an executable. When this executable is launched, it creates the
 * necessary threads and timers. Then, it calls the function _run_main() as a low-priority task.
 * This function is undefined by RTDAL. At compile time, RTDAL <b> must be linked </b> with an
 * object (or library) containing at least the _run_main() function.
 *
 * These pages describe the RTDAL API. Please click on <b>Modules</b> to see the API functions.
 */

/** \addtogroup process
 *
 * An RTDAL Synchronous High-Priority Task is called a \b process. A process is defined as a
 * dynamically loaded shared library <b>containing the symbol _run_cycle()</b>. One thread per
 * processing core is created in each processor. Each thread calls the functions _run_cycle() of
 * each process one after another, given the order specified during the process creation.
 *
 * A new process is created using rtdal_process_new() and is removed from the system with
 * rtdal_process_remove(). The process can be removed only from an external task, not within the
 * process execution thread. A call to rtdal_process_run() enables the process execution which
 * begins in the next timeslot. A call to rtdal_process_stop() stops its execution in the next
 * timeslot but does not remove the process from the system.
 *
 * The function rtdal_process_new() establishes a callback function. This function is called
 * when the process causes an error (e.g. RTFAULT or SIG_RECV) or when the function
 * rtdal_process_seterror() is called (see rtdal_processerrors_t). If the processerror is different
 * than FINISH_OK and rtdal_process_isrunning() is true, RTDAL calls the callback function and does
 * not run the process _run_cycle() function. This offers some sort of rudimentary communication
 * mechanism between the process and another low-priority task.
 *
 * Each process <b>must be associated</b> with a process_group_id, which is a non-negative integer
 * number lower than MAX_PROCESS_GROUP_ID. When a process causes an error RTFAULT or SIG_RECV,
 * RTDAL will call the finish_callback function only once, and this function will receive as a
 * parameter a handler to the processes with lowest exec_position and pipeline_id number that
 * caused one of these errors. A call to rtdal_process_group_notified() resets the state and
 * enables another call to finish_callback.
 *
 * <b> RTFAULT ERROR </b>
 *
 * An RTFAULT error is generated when a process executes for longer than a time slot length. Say the
 * process is running at timeslot n. If at the beginning of timeslot n+1 the process is still running,
 * it will be killed and the finish_callback function will be called. The core execution thread will
 * then continue with the next process in the pipeline.
 *
 */

/** \addtogroup task
 * An asynchronous low-priority task is a task that executes with no periodicy and lower priority
 * than the synchronous high-priority task.
 *
 * These tasks are created using rtdal_task_new(). If the first argument of rtdal_task_new() is non-null
 * the function rtdal_task_wait() or rtdal_task_wait_nb() <b> must be used </b> to deallocate the
 * task resources from the system.
 */

/** \addtogroup period
 * A synchronous low-priority task is called periodically synchronous to the time slot. Conversely
 * to high-priority synchronous tasks, these are called within a separate thread with lower priority.
 * The execution period is defined in multiples of the time slot.
 *
 * Since creating a new thread consumes resources, it is <b> highly recommended </b> to use this functions
 * with a high periodicy, either due to a large time slot or due to a large multiplicity factor (argument
 * period in rtdal_periodic_add() function).
 *
 * A periodic task is created using rtdal_periodic_add() and removed from the system using
 * rtdal_periodic_remove().
 *
 * If the task does not return from the function before the next execution time arrives, a new
 * thread will be created which will run the function again.
 */

/** \addtogroup itf
 *
 * RTDAL interfaces allow the communication between different tasks (high or low priority, synchronous or not).
 *
 * Interfaces can be external or internal. Internal interfaces communicate tasks running in processors
 * that share memory whereas tasks that not share a common memory need to use external interfaces.
 *
 * Interfaces can have an associated <b> delay </b>, which is a non-negative integer number. If delay
 * is non-null, a packet generated at time slot <i>n</i> is not received until timeslot <i>n+delay</i>.
 *
 * Two or more tasks can communicate with an internal interface if all use the same r_itf_t handler with the
 * send/recv or push/pop functions. Only one task can create the interface and need to share the
 * handler with the other tasks, for instance using a global memory.
 *
 * External or physical interfaces are defined in the platform configuration file and are associate with a name.
 * Two or more tasks using rtdal_itfphysic_get() to obtain a handler to a physical interface with the same
 * name can send and receive data.
 *
 * TODO: Physical interfaces are not yet implemented.
 *
 * There are two kinds of internal interfaces:
 * 	- SPSCQ is a single producer single consumer wait-free queue.
 * 	- MPMCQ is a multi producer multi consumer lock-free queue which may be used if more than one
 * 	task has to read or write from the interface. TODO: This is currently under development
 *
 * All interfaces share the same interface, defined by the family of functions rtdal_itf_*. Either
 * external or internal interfaces are managed with the same type of handler: r_itf_t. What determines
 * the kind of interface is the function used to obtain the handler.
 *
 * For instance:
 *
 * \code{.c}
 *
 * r_itf_t my_int, my_ext;
 *
 * // this creates an SPSC queue
 * my_int = rtdal_itfspscq_new(10,100);
 * // this attaches to an external physical interface
 * my_ext = rtdal_itfphysic_get("any_address_string");
 *
 * // and now we are sending data through any of them
 * rtdal_itf_send(my_int, buffer, buffer_sz);
 *
 * rtdal_itf_send(my_ext, buffer, buffer_sz);
 *
 * \endcode
 *
 *
 *
 * RTDAL interfaces support two kinds of operation, regardless of their particular implementation.
 * normal or zero-copy. In normal mode, the transmitter task supplies a buffer whose contents are copied
 * to an internal buffer. The receiver then copies these internal buffer contents to a buffer in his
 * application. In zero-copy mode, the transmitter task obtains a buffer where the packet contents are
 * copied. The receiver task reads from the same buffer, avoiding memory transfers.
 *
 * The zero-copy mode needs to request a pointer prior to pushing a packet into the queue. Similarly,
 * the receiver has to release the pointer after poping from the queue. The following example illustrates
 * the zero-copy operation of interfaces:
 *
 * This is the transmitter side:
 * \code{.c}
 *
 * int *ptr;
 *
 * rtdal_itf_request(my_itf, &ptr);
 *
 * for (i=0;i<len;i++) {
 * 	ptr[i] = fill_data(i);
 * }
 *
 * rtdal_itf_push(my_itf, len);
 *
 * \endcode
 *
 * And this is the receiver side:
 * \code{.c}
 *
 * int *ptr;
 *
 * rtdal_itf_pop(my_itf, &ptr, &len);
 *
 * for (i=0;i<len;i++) {
 * 	process_data(i);
 * }
 *
 * rtdal_itf_release(my_itf);
 *
 * \endcode
 *
 * In some cases, the interface may not support one of the modes natively. In this case, the implementation
 * has to simulate the desired behavior. For instance, if a physical interfaces does not support
 * zero-copy operation, it may dynamically allocate a buffer which is given to the transmitter and then
 * transfered to the receiver. On the other hand, SPSC queues only implement the zero-copy mode.
 * When the user uses the normal mode, the implementation is using the zero-copy mode internally.
 *
 * The normal mode is operated using the rtdal_itf_send() and rtdal_itf_recv() functions.
 *
 * The zero-copy mode is operated using the rtdal_itf_push(), rtdal_itf_pop(), rtdal_itf_request() and
 * rtdal_itf_release() functions.
 */


#endif
