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

#ifndef oesr_H_
#define oesr_H_

#include "oesr_types.h"

/**@defgroup module_functions Module functions
 *
 * @{
 */
/**
 * This function is called once every time slot to perform the DSP signal processing. It is subject
 * to real-time deadline constraints, that is, the execution time must be lower than a time slot and
 * the required resources shall be modeled in the waveform model description.
 *
 * In this function, the user shall receive data from the input interfaces, process it and
 * send the result through the output interfaces.
 *
 * \returns 0 on success or -1 on error. On error, OESR will stop the entire waveform */
int Run(void *context);

/**
 * This function is called during the initialization (INIT) phase. This function is not subject
 * to real-time constraints. Rather, OESR creates a low-priority task that calls the Init() function
 * for each module, one after another.
 *
 * In this function, the user shall create the interfaces, declare public variables and perform
 * other initialization tasks, like look-up tables or filter coefficients computation, for instance.
 *
 * If this function returns 0, it will be called again after having called the other modules. This is
 * required to initialize interfaces since they are created by the writer module only. Since the
 * initialization order is unknown, the interface may not be created when the reader calls
 * swapi_itf_create() and shall sleep until it is created. Note that if this feature is used,
 * the Init() function must be reentrant, that is, it needs to track which resources have already been
 * opened and which not, in order to avoid opeining the same resource twice.
 *
 * \return 1 on success, 0 to sleep called in the next time slot or -1 on error, which stops the waveform */
int Init(void *context);

/**
 * The Stop() function is called once during waveform stop (STOP phase). It is used to deallocate
 * the resources allocated during Init() and to close the created interfaces.
 * \returns 0 on success or -1 on error
 */
int Stop(void *context);
/**@} */



/**@defgroup error Error functions
 * The error functions are used to print the errors produced during a call to an OESR function.
 *
 * When a function returns -1, the error is stored in a variable which is obtained using
 * oesr_error_code() and equals one of the OESR_ERROR_* constants.
 *
 * An error message can be obtained using the functions oesr_error_print() or the macro oesr_perror().
 *
 * @{
 */

#define OESR_ERROR_INVAL 		1
#define OESR_ERROR_RTDAL 		2
#define OESR_ERROR_OTHER 		3
#define OESR_ERROR_NOSPACE 		4
#define OESR_ERROR_LARGE 		5
#define OESR_ERROR_NOTFOUND 	6
#define OESR_ERROR_MODNOTFOUND 	7
#define OESR_ERROR_NOTREADY 	8

#define oesr_perror(msg) oesr_error_print(ctx,""); aerror(msg)
char *oesr_error_string(void *ctx);
int oesr_error_code(void *ctx);
void oesr_error_print(void *context, const char *user_message);
/**@} */

/**@defgroup base Other functions
 * @{
 */
int oesr_tstamp(void *context);
int oesr_tslot_length(void *context);
int oesr_exit(void *context);
char *oesr_module_name(void *context);
int oesr_module_id(void *context);
/**@} */

/**@defgroup itf Interface functions
 * @{
 */
#define OESR_ITF_DEFAULT_MSG 50
typedef enum {
	ITF_READ, ITF_WRITE
}oesr_itf_mode_t;
itf_t oesr_itf_create(void *context, int port_idx, oesr_itf_mode_t mode, int size);
int oesr_itf_close(itf_t itf);
int oesr_itf_write(itf_t itf, void* buffer, int size);
int oesr_itf_read(itf_t itf, void* buffer, int size);
int oesr_itf_status(itf_t itf);
int oesr_itf_ptr_request(itf_t itf, void **ptr);
int oesr_itf_ptr_release(itf_t itf);
int oesr_itf_ptr_put(itf_t itf, int len);
int oesr_itf_ptr_get(itf_t itf, void **ptr, int *len);
/**@} */

/**@defgroup var Public variables and parameters functions
 * @{
 */
typedef enum {
	VAR_INT, VAR_FLOAT, VAR_STRING
}oesr_var_type_t;

var_t oesr_var_create(void *context, char* name, void *ptr, int size);
int oesr_var_close(void *context, var_t var);
var_t oesr_var_param_get(void *context, char *name);
int oesr_var_param_value(void *context, var_t parameter, void* value, int size);
oesr_var_type_t oesr_var_param_type(void *context, var_t parameter);
/**@} */

/**@defgroup counter Counters
 * @{
 */
counter_t oesr_counter_create(void *context, char* name);
int oesr_counter_close(counter_t counter);
int oesr_counter_start(counter_t counter);
int oesr_counter_stop(counter_t counter);
int oesr_counter_usec(counter_t counter);
/**@} */

/**@defgroup log Logs
 * @{
 */
log_t oesr_log_create(void *context, char* name);
int oesr_log_close(log_t log);
int oesr_log_write(log_t log, char *str);
int oesr_log_printf(log_t log, const char *fmt, ...);
/**@} */


/** \mainpage ALOE++ Operating Environment for Software-defined Radio (OESR) API
 *
 * The OESR, on the other hand, is built on top of the RTDAL. This allows future portability to other
 * platforms (currently, RTDAL employs the POSIX interface and gcc atomic functions).
 * OESR provides functionalities specifically tailored for SDR applications:
 *  * Automatic mapping of waveforms to a set of processing cores (distributed or in a multi-core, or both).
 *  * Location-transparent inter-module communications.
 *  * Global variables and parameters configuration/visualization
 *  * Logs, counters and others.
 *
 * In order to facilitate DSP modules development and ALOE++ usage, the following set of tools have been developed:
 *  * OESR Skeleton.
 *  * Stand-alone program generation.
 *  * MATLAB/Octave automatic MEX-file generation.
 *
 *  An example of the OESR Skeleton is found in the modrep_default/template directory.
 *  The user only needs to worry about coding the DSP algorithm. The skeleton will manage the
 *  interfaces and control variables. Read also this guide for help on creating a new DSP module.
 *  For documentation on how to write OESR Skeleton based DSP blocks, refer to the modrep_default/template
 *  module documentation.
 *
 *  An OESR DSP module is compiled as a shared library and includes the OESR API static library. The
 *  module source code must implement the following functions
 *   * Init()
 *   * Run()
 *   * Stop()
 *
 *  What this functions have to implement is explained in the Module Functions section (under Modules).
 *
 *  These pages describe the OESR API used by the DSP modules to interface ALOE. Please click on
 *  <b>Modules</b> to view the functions provided by the API.
 *
 */

/** \addtogroup module_functions
 *
 * These set of functions <b>must be implemented</b> by the DSP module source code. See modrep_default/template_plain
 * for an example. They will be called by the OESR according to the waveform status.
 *
 * All functions receive as a parameter a pointer to the OESR context. This pointer has to be passed
 * to some of the OESR API functions.
 *
 */

/** \addtogroup var
 *
 * Variables and parameters are module variables accessible by other modules or from the OESR Manager API.
 * For instance, a module may estimate the SNR and make the variable "public". Then another module
 * can access it (not yet implemented). An external program that manages the ALOE modem may, for instance,
 * reconfigure the waveform mode when the SNR is below certain threshold.
 *
 * Parameters are subset of variables which have an initialization value, specified in the waveform
 * .app file. They are a subset in the sense that a parameter is also publicly accessible. Parameters
 * are used to configure the functionality of the DSP module. For instance the DFT size or the length
 * of the CRC are examples of parameters. They are specified in the module documentation, as the
 * modrep_default/template example illustrates.
 *
 * This documentation has to define whether a change of a parameter can be done during the RUN
 * phase or only during the INIT phase. If a parameter can be changed during the RUN phase, it implies
 * that a reconfiguration can be done in real-time. That is, that the execution time of the module
 * is unaffected by a change of parameter. On the other hand, a parameter can only be modified during
 * the INIT phase if a change of its value implies an execution time longer that the specified in the
 * model .app.
 *
 * A parameter value is obtained using the function oesr_var_param_value(). This function gives the
 * value associated with the current <b>waveform mode</b>. The waveform mode is defined in the
 * waveform .app file. It defines a value for each module parameter. Each parameter, on the other hand,
 * has a different value for each different mode. This allows the dynamic reconfiguration of the waveform
 * from one mode to another.
 */

/**\addtogroup counter
 *
 * A counter shall be used by a user to measure time intervals. oesr_counter_create() returns
 * a counter_t object which is passed as a first parameter to oesr_counter_start() and
 * oesr_counter_stop() functions, which start and stop the counter, respectively. A counter is
 * associated with a public module variable which can be accessed from the oesr_man interface. The
 * variable value is updated during a call to oesr_counter_stop(), saving the elapsed time between
 * successive calls to oesr_counter_start() and oesr_counter_stop(), in microseconds.
 * After calling oesr_counter_stop(), the function oesr_counter_usec() returns the elapsed time,
 * in microseconds.
 *
 * Example:
 * \code{.c}
 *
 * counter_t mycounter = oesr_counter_create(context, "my counter");
 *
 * oesr_counter_start(mycounter);
 * do_some_work();
 * oesr_counter_stop(mycounter);
 * printf("Elapsed time is %d usec\n",oesr_counter_usec(mycounter));
 *
 * \endcode
 */

/**\addtogroup log
 *
 * A log may be used by a module to write text to a file in the filesystem. Whether the filesystem is
 * local or remote depends on OESR and is abstracted.
 *
 * Create a log using oesr_log_create() and then use oesr_log_write() or oesr_log_printf() to write
 * strings to it.
 */

/**\addtogroup itf
 *
 * An interface is used by modules to exchange data packets (i.e. samples) asynchronously.
 *
 * An interface is created using oesr_itf_create(), which returns a handler pointer of type itf_t
 * on success. This handler is passed to the oesr_itf_*() functions as the first parameter, in
 * order to operate with interface.
 *
 * There are two modes to transmit/receive data. The oesr_itf_write() function write a given amount
 * of bytes from a buffer provided by the user to the interface. Memory copies can be avoided using
 * the oesr_itf_ptr* collection of functions. With this mode, the transmitter obtains a buffer
 * shared with the receiver. Then both modules write and read avoiding memory copies. The buffer is
 * released by the receiver after the samples have been processed, avoiding other modules to use
 * it meanwhile.
 *
 * The family of oesr_itf_ptr_* functions may be used for more efficient interface usage:
 * they are designed for zero-copy packet communication.
 *
 * The transmitter obtains a buffer using the function oesr_itf_ptr_request(). It fills the contents
 * with the samples to transmitted and then calls oesr_itf_ptr_put() to make the packet available
 * to the receiver. The receiver obtains the buffer address using the function oesr_itf_ptr_get().
 * After the samples have been processed, a final call to oesr_itf_ptr_release() allows the packet
 * to be reused again by the transmitter.
 *
 * These functions give the user direct access to the internal RTDAL memory. The buffers are
 * automatically allocated for the size passed as a parameter to the oesr_itf_create() function.
 * The user MUST ensure that this size is not exceed.
 *
 * Example:
 *
 * This would be the transmitter module:
 * \code{.c}
 *
 * int *ptr;
 *
 * oesr_itf_ptr_request(my_itf, &ptr);
 *
 * for (i=0;i<len;i++) {
 * 	ptr[i] = fill_data(i);
 * }
 *
 * oesr_itf_ptr_put(my_itf, len);
 *
 * \endcode
 *
 * And this is the receiver side:
 * \code{.c}
 *
 * int *ptr;
 *
 * oesr_itf_ptr_get(my_itf, &ptr, &len);
 *
 * for (i=0;i<len;i++) {
 * 	process_data(i);
 * }
 *
 * oesr_itf_ptr_release(my_itf);
 *
 * \endcode
 *
 * More information on OESR interfaces is given in the RTDAL documentation (interfaces section).
 */

#endif
