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

#include <stddef.h>
#include <assert.h>
#include "defs.h"
#include "oesr.h"
#include "str.h"
#include "rtdal.h"
#include "oesr_context.h"
#include "nod_waveform.h"
#include "waveform.h"



/**
 *
 * The oesr_itf_create() function initializes the interface with the rtdal. A pair of
 * transmitter/receiver modules use the same rtdal interface to communicate each other. This interface
 * is created by the transmitter. The receiver then attaches to the same interface. Since the
 * order of creation is unknown, the interface might not yet be created by the transmitter. In this
 * case, the oesr_error_code is set to OESR_ERROR_NOTREADY and the transmitter may try to call
 * again the oesr_itf_create() function in the next timeslot.
 *
 * \param context Pointer to the oesr context
 * \param port_idx Interface local port number
 * \param mode READ or WRITE
 * \param size Maximum size of the packets that can be transmitted through the interface (used only
 * by the transmitter side)
 *
 * \return Non-null handler on success or null on error.
 *
 */
itf_t oesr_itf_create(void *context, int port_idx, oesr_itf_mode_t mode,
		int size) {
	oesr_context_t *ctx = context;
	nod_module_t *module = ctx->module;
	sdebug("context=0x%x, module_id=%d, port_idx=%d, mode=%d, size=%d inputs=%d outputs=%d\n",context,module->parent.id,
			port_idx, mode, size,module->parent.nof_inputs,module->parent.nof_outputs);
	r_itf_t rtdal_itf;
	interface_t *nod_itf = NULL;

	OESR_ASSERT_PARAM_P(module);
	OESR_ASSERT_PARAM_P(port_idx>=0);

	if (mode == ITF_WRITE) {
		if (port_idx >= module->parent.nof_outputs) {
			OESR_SETERROR(OESR_ERROR_NOTFOUND);
			return NULL;
		}
		nod_itf = &module->parent.outputs[port_idx];
	} else {
		if (port_idx >= module->parent.nof_inputs) {
			OESR_SETERROR(OESR_ERROR_NOTFOUND);
			return NULL;
		}
		nod_itf = &module->parent.inputs[port_idx];
	}

	if (nod_itf->physic_itf_id != 0) {
		/* is external */
		rtdal_itf = (r_itf_t) rtdal_itfphysic_get_id(nod_itf->physic_itf_id);
		if (!rtdal_itf) {
			OESR_HWERROR("rtdal_itf_physic_get_id");
		}
	} else {
		/* is internal */
		if (mode == ITF_WRITE) {
			rtdal_itf = (r_itf_t) rtdal_itfspscq_new(OESR_ITF_DEFAULT_MSG,
					size);
			if (!rtdal_itf) {
				OESR_HWERROR("rtdal_itfqueue");
				return NULL;
			}
		} else {
			sdebug("remote_id=%d, remote_idx=%d\n",nod_itf->remote_module_id,
					nod_itf->remote_port_idx);
			nod_waveform_t *waveform = module->parent.waveform;
			nod_module_t *remote = nod_waveform_find_module_id(waveform,
					nod_itf->remote_module_id);
			if (!remote) {
				OESR_SETERROR(OESR_ERROR_MODNOTFOUND);
				return NULL;
			}
			sdebug("remote found\n",0);
			if (nod_itf->remote_port_idx > remote->parent.nof_outputs) {
				OESR_SETERROR(OESR_ERROR_NOTFOUND);
				return NULL;
			}
			sdebug("valid\n",0);
			rtdal_itf = remote->parent.outputs[nod_itf->remote_port_idx].hw_itf;
			if (!rtdal_itf) {
				OESR_SETERROR(OESR_ERROR_NOTREADY);
				return NULL;
			}
		}
	}
	nod_itf->hw_itf = rtdal_itf;
	sdebug("module_id=%d hitf=0x%x\n",module->parent.id,rtdal_itf);

	return (itf_t) nod_itf;
}

/**  The oesr_itf_close() function closes an interface previously created by oesr_itf_create().
 * The interface shall not be used again after calling this function.
 *
 * \param itf Handler returned by the oesr_itf_create() function.
 *
 * \return Zero on success or -1 on error
 *
 */
int oesr_itf_close(itf_t itf) {
	assert(itf);
	interface_t *x = (interface_t*) itf;
	return rtdal_itf_remove(x->hw_itf);
}

/**
 * oesr_itf_write() copies size bytes of the memory pointed by buffer to a packet which is sent
 * through the interface. If size is larger than the size used in oesr_itf_create(), no data is
 * copied and -1 is returned. If there is not enough space in the interface to send the packet,
 * 0 is returned. When the function returns, the user may overwrite the contents of
 * the memory pointed by buffer.
 *
 * \param itf Handler returned by the oesr_itf_create() function.
 * \param buffer Pointer to the memory to copy to the interface
 * \param size Number of bytes to copy
 *
 * \return 1 if all the packet was sent, 0 if the packet was not sent or -1 on error.
 *
 */
int oesr_itf_write(itf_t itf, void* buffer, int size) {
	assert(itf);
	interface_t *x = (interface_t*) itf;
	return rtdal_itf_send(x->hw_itf,buffer,size);
}

/**
 * oesr_itf_read() copies up to size bytes of the memory pointed by buffer from the packet
 * received from the interface. If size is smaller than the received packet length, only size
 * bytes will be copied. If no packet is pending in the interface, zero is returned.
 *
 * \param itf Handler returned by the oesr_itf_create() function.
 * \param buffer Pointer to the memory to copy to the interface
 * \param size Size of the buffer pointed by buffer
 *
 * \return The number of bytes copied to the buffer (zero or positive number) on succes,
 * or -1 on error
 */
int oesr_itf_read(itf_t itf, void* buffer, int size) {
	assert(itf);
	interface_t *x = (interface_t*) itf;
	return rtdal_itf_recv(x->hw_itf,buffer,size);
}

/**
 * oesr_itf_status() returns the number of bytes pending in the interface.
 *
 * \param itf Handler returned by the oesr_itf_create() function.
 *
 * \return The number of bytes pending in the interface or -1 on error
 *
 */
int oesr_itf_status(itf_t itf) {
	aerror("Not yet implemented");
	return -1;
}

/**
 * Requests a pointer for zero-copy interface utilization.
 *
 * \details In the current implementation, internal interfaces employ a SPSP wait-free queue.
 * Therefore, one transmitter module and one receiver module can use the same interface.
 *
 * \param itf Handler returned by the oesr_itf_create() function.
 * \param ptr Address of the buffer where the samples to transmit must be written.
 *
 * \return 1 if a pointer was successfully allocated, 0 if there is no space in the interface
 * or -1 on error
 *
 */
int oesr_itf_ptr_request(itf_t itf, void **ptr) {
	assert(itf);
	interface_t *x = (interface_t*) itf;
	return rtdal_itf_request(x->hw_itf, ptr);
}


/**
 * Releases a pointer received using oesr_itf_ptr_get() after the contents have been
 * processed.
 *
 * \param itf Handler returned by the oesr_itf_create() function.
 *
 * \return 1 on success, 0 if the packet could not be released, or -1 on error.
 *
 */
int oesr_itf_ptr_release(itf_t itf) {
	assert(itf);
	interface_t *x = (interface_t*) itf;
	return rtdal_itf_release(x->hw_itf);
}


/**
 * Sends a buffer obtained by oesr_itf_ptr_request() after the samples have been written to it.
 *
 * \param itf Handler returned by the oesr_itf_create() function.
 * \param len Number of useful bytes written to the buffer
 *
 * \return 1 on success, 0 if the packet could not be sent or -1 on error.
 *
 */
int oesr_itf_ptr_put(itf_t itf, int len) {
	assert(itf);
	interface_t *x = (interface_t*) itf;
	return rtdal_itf_push(x->hw_itf, len);
}


/**
 * Receives a buffer from an interface.
 *
 * \param itf Handler returned by the oesr_itf_create() function.
 * \param ptr Pointer where the received packet address will be stored
 * \param len Pointer to where the number of useful received samples will be saved.
 *
 * \return 1 on success, 0 if there are no packets pending in the interface or -1 on error.
 *
 */
int oesr_itf_ptr_get(itf_t itf, void **ptr, int *len) {
	assert(itf);
	interface_t *x = (interface_t*) itf;
	return rtdal_itf_pop(x->hw_itf, ptr, len);
}
