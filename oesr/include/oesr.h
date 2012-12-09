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
 * @{
 */
/**\brief Return 0 on success or -1 to stop the waveform */
int Run(void *context);
/**\brief Return 1 on success, 0 to be called in the next timeslot or -1 to stop the waveform */
int Init(void *context);
/**\brief Return 0 on success or -1 on error */
int Stop(void *context);
/**@} */

#define OESR_ITF_DEFAULT_MSG 50


/**@defgroup error Error functions
 * @{
 */
#define oesr_perror(msg) oesr_error_print(ctx,""); aerror(msg)
int oesr_error_code(void *ctx);
void oesr_error_print(void *context, const char *user_message);
/**@} */

/**@defgroup base Basic API functions
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
 * OESR is an operating environment based on RTDAL for distributed DSP for SDR.
 *
 * This is the API for DSP modules.
 */

#endif
