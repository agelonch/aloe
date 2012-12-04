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
#include "defs.h"
#include "swapi.h"
#include "str.h"
#include "hwapi.h"
#include "swapi_context.h"
#include "nod_waveform.h"
#include "waveform.h"



/** \brief An interface is used by modules to exchange data packets (with samples) asynchronously.
 *
 * An interface is created using swapi_itf_create(), which returns a handler pointer of type itf_t
 * on success. This handler is passed to the swapi_itf_*() functions as the first parameter, in
 * order to operate with interface.
 *
 * There are two modes to transmit/receive data. The swapi_itf_write() function write a given amount
 * of bytes from a buffer provided by the user to the interface. Memory copies can be avoided using
 * the swapi_itf_ptr* collection of functions. With this mode, the transmitter obtains a buffer
 * shared with the receiver. Then both modules write and read avoiding memory copies. The buffer is
 * released by the receiver after the samples have been processed, avoiding other modules to use
 * it meanwhile.
 *
 * The swapi_itf_create() function initializes the interface with the hwapi. A pair of
 * transmitter/receiver modules use the same hwapi interface to communicate each other. This interface
 * is created by the transmitter. The receiver then attaches to the same interface. Since the
 * order of creation is unknown, the interface might not yet be created by the transmitter. In this
 * case, the swapi_error_code is set to SWAPI_ERROR_NOTREADY and the transmitter may try to call
 * again the swapi_itf_create() function in the next timeslot.
 *
 * \param context Pointer to the swapi context
 * \param port_idx Interface local port number
 * \param mode READ or WRITE
 * \param size Maximum size of the packets that can be transmitted through the interface (used only
 * by the transmitter side)
 *
 * \return Non-null handler on success or null on error.
 *
 */
itf_t swapi_itf_create(void *context, int port_idx, swapi_itf_mode_t mode,
		int size) {
	swapi_context_t *ctx = context;
	nod_module_t *module = ctx->module;
	sdebug("context=0x%x, module_id=%d, port_idx=%d, mode=%d, size=%d inputs=%d outputs=%d\n",context,module->parent.id,
			port_idx, mode, size,module->parent.nof_inputs,module->parent.nof_outputs);
	h_itf_t hwapi_itf;
	interface_t *nod_itf = NULL;

	SWAPI_ASSERT_PARAM_P(module);
	SWAPI_ASSERT_PARAM_P(port_idx>=0);

	if (mode == ITF_WRITE) {
		if (port_idx >= module->parent.nof_outputs) {
			SWAPI_SETERROR(SWAPI_ERROR_NOTFOUND);
			return NULL;
		}
		nod_itf = &module->parent.outputs[port_idx];
	} else {
		if (port_idx >= module->parent.nof_inputs) {
			SWAPI_SETERROR(SWAPI_ERROR_NOTFOUND);
			return NULL;
		}
		nod_itf = &module->parent.inputs[port_idx];
	}

	if (nod_itf->physic_itf_id != 0) {
		/* is external */
		hwapi_itf = (h_itf_t) hwapi_itfphysic_get_id(nod_itf->physic_itf_id);
		if (!hwapi_itf) {
			SWAPI_HWERROR("hwapi_itf_physic_get_id");
		}
	} else {
		/* is internal */
		if (mode == ITF_WRITE) {
			hwapi_itf = (h_itf_t) hwapi_itfqueue_new(SWAPI_ITF_DEFAULT_MSG,
					size);
			if (!hwapi_itf) {
				SWAPI_HWERROR("hwapi_itfqueue");
				return NULL;
			}
		} else {
			sdebug("remote_id=%d, remote_idx=%d\n",nod_itf->remote_module_id,
					nod_itf->remote_port_idx);
			nod_waveform_t *waveform = module->parent.waveform;
			nod_module_t *remote = nod_waveform_find_module_id(waveform,
					nod_itf->remote_module_id);
			if (!remote) {
				SWAPI_SETERROR(SWAPI_ERROR_MODNOTFOUND);
				return NULL;
			}
			sdebug("remote found\n",0);
			if (nod_itf->remote_port_idx > remote->parent.nof_outputs) {
				SWAPI_SETERROR(SWAPI_ERROR_NOTFOUND);
				return NULL;
			}
			sdebug("valid\n",0);
			hwapi_itf = remote->parent.outputs[nod_itf->remote_port_idx].hw_itf;
			if (!hwapi_itf) {
				SWAPI_SETERROR(SWAPI_ERROR_NOTREADY);
				return NULL;
			}
		}
	}
	nod_itf->hw_itf = hwapi_itf;
	sdebug("module_id=%d hitf=0x%x\n",module->parent.id,hwapi_itf);

	return (itf_t) nod_itf;
}

/** \brief The swapi_itf_close() function closes an interface previously created by swapi_itf_create().
 * The interface shall not be used again after calling this function.
 *
 * See swapi_itf_create() for an introduction to swapi interfaces.
 *
 * \param itf Handler returned by the swapi_itf_create() function.
 *
 * \return Zero on success or -1 on error
 *
 */
int swapi_itf_close(itf_t itf) {
	aassert(itf);
	sdebug("itf_id=%d\n",itf->id);
	interface_t *x = (interface_t*) itf;
	return hwapi_itf_remove(x->hw_itf);
}

/** \brief swapi_itf_write() copies size bytes of the memory pointed by buffer to a packet which is sent
 * through the interface. If size is larger than the size used in swapi_itf_create(), no data is
 * copied and -1 is returned. If there is not enough space in the interface to send the packet,
 * 0 is returned. When the function returns, the user may overwrite the contents of
 * the memory pointed by buffer.
 *
 * See swapi_itf_create() for an introduction to swapi interfaces.
 *
 * \param itf Handler returned by the swapi_itf_create() function.
 * \param buffer Pointer to the memory to copy to the interface
 * \param size Number of bytes to copy
 *
 * \return 1 if all the packet was sent, 0 if the packet was not sent or -1 on error.
 *
 */
int swapi_itf_write(itf_t itf, void* buffer, int size) {
	aassert(itf);
	sdebug("itf_id=%d, buffer=0x%x, size=%d\n",itf->id, buffer,size);
	aassert(buffer);
	aassert(size>=0);
	interface_t *x = (interface_t*) itf;
	return hwapi_itf_send(x->hw_itf,buffer,size);
}

/** \brief swapi_itf_read() copies up to size bytes of the memory pointed by buffer from the packet
 * received from the interface. If size is smaller than the received packet length, only size
 * bytes will be copied. If no packet is pending in the interface, zero is returned.
 *
 * See swapi_itf_create() for an introduction to swapi interfaces.
 *
 * \param itf Handler returned by the swapi_itf_create() function.
 * \param buffer Pointer to the memory to copy to the interface
 * \param size Size of the buffer pointed by buffer
 *
 * \return The number of bytes copied to the buffer (zero or positive number) on succes,
 * or -1 on error
 */
int swapi_itf_read(itf_t itf, void* buffer, int size) {
	aassert(itf);
	sdebug("itf_id=%d, buffer=0x%x, size=%d\n",itf->id, buffer,size);
	aassert(buffer);
	aassert(size>=0);
	interface_t *x = (interface_t*) itf;
	return hwapi_itf_recv(x->hw_itf,buffer,size);
}

/** \brief swapi_itf_status() returns the number of bytes pending in the interface.
 *
 * See swapi_itf_create() for an introduction to swapi interfaces.
 *
 * \param itf Handler returned by the swapi_itf_create() function.
 * \param buffer Pointer to the memory to copy to the interface
 * \param size Size of the buffer pointed by buffer
 *
 * \return The number of bytes pending in the interface or -1 on error
 *
 */
int swapi_itf_status(itf_t itf) {
	aerror("Not yet implemented");
	return -1;
}

/** \brief Requests a pointer for zero-copy interface utilization.
 *
 * See swapi_itf_create() for an introduction to swapi interfaces.
 *
 * The family of swapi_itf_ptr_* functions may be used for more efficient interface usage: they are designed
 * for zero-copy packet communication. A call to swapi_itf_pkt_request() returns a pointer to a structure
 * of type pkt_t:
 *
 * typedef struct {
 * 	int len;
 * 	void *data;
 * }pkt_t;
 *
 * where len indicates the packet length and data is a pointer to the packet buffer. The size of this
 * buffer equals the parameter size passed to the swapi_itf_create() function.
 * The user should not read/write more data from/to this buffer, otherwise the result is unexpected.
 *
 * ALOE uses the field len when the interface opposite side uses the functions swapi_itf_read/write.
 * Then len should be equal to the number of useful bytes in the packet. If all modules in the waveform
 * use the family of functions swapi_itf_pkt_* then len can have another meaning, like bits/bytes/words
 * as soon as the transmitter and receiver agree.
 *
 * A successive call to swapi_itf_ptr_put() sends the packet to the receiver without copying the
 * buffer contents. The receiver will read the samples from the same buffer. Therefore, after a call
 * to swapi_itf_ptr_put() the transmitter can NOT use the buffer and may request another packet using
 * swapi_itf_ptr_request().
 *
 * The receiver uses swapi_itf_ptr_get() to receive the packet (if any pending in the interface).
 * After it has processed all the samples, a call to swapi_itf_ptr_release() releases the packet,
 * which enables to reuse the buffer in a future call to swapi_itf_ptr_request().
 *
 * \param itf Handler returned by the swapi_itf_create() function.

 * \return A non-null pkt_t pointer on success or null on error.
 *
 */
pkt_t* swapi_itf_pkt_request(itf_t itf) {
	aassert_p(itf);
	sdebug("itf_id=%d\n",itf->id);
	interface_t *x = (interface_t*) itf;
	return (pkt_t*) hwapi_itf_request_pkt(x->hw_itf);
}


/** \brief Releases a pointer received using swapi_itf_ptr_get() after the contents have been
 * processed.
 *
 * See swapi_itf_create() for an introduction to swapi interfaces.
 *
 * See swapi_itf_pkt_request() for documentation on the swapi_itf_ptr_* family of functions.
 *
 * \param itf Handler returned by the swapi_itf_create() function.
 *
 * \return 1 on success, 0 if the packet could not be released, or -1 on error.
 *
 */
int swapi_itf_pkt_release(itf_t itf, pkt_t *pkt) {
	aassert(itf);
	sdebug("itf_id=%d, pkt=0x%x\n",itf->id, pkt);
	aassert(pkt);
	interface_t *x = (interface_t*) itf;
	return hwapi_itf_release_pkt(x->hw_itf, (h_pkt_t*) pkt);
}


/** \brief Sends a buffer obtained by swapi_itf_ptr_new() after the samples have been written to it.
 *
 * See swapi_itf_create() for an introduction to swapi interfaces.
 *
 * See swapi_itf_pkt_request() for documentation on the swapi_itf_ptr_* family of functions.
 *
 * \param itf Handler returned by the swapi_itf_create() function.
 * \param ptr Pointer returned by swapi_itf_ptr_new()
 * \param size Number of bytes to send (and have been written to the buffer)
 *
 * \return 1 on success, 0 if the packet could not be sent or -1 on error.
 *
 */
int swapi_itf_pkt_put(itf_t itf, pkt_t *pkt) {
	aassert(itf);
	sdebug("itf_id=%d, pkt=0x%x\n",itf->id, pkt);
	aassert(pkt);
	interface_t *x = (interface_t*) itf;
	return hwapi_itf_put_pkt(x->hw_itf, (h_pkt_t*) pkt);
}


/** \brief Receives a buffer from an interface.
 *
 * See swapi_itf_create() for an introduction to swapi interfaces.
 *
 * See swapi_itf_pkt_request() for documentation on the swapi_itf_ptr_* family of functions.
 *
 * \param itf Handler returned by the swapi_itf_create() function.
 * \param len The function stores in the integer pointed by len the length of the received packet,
 * in bytes (or number of bytes that should be read from the buffer)
 *
 * \return A non-null pkt_t pointer on success or null on error.
 *
 */
pkt_t* swapi_itf_pkt_get(itf_t itf) {
	aassert_p(itf);
	sdebug("itf_id=%d\n",itf->id);
	interface_t *x = (interface_t*) itf;
	return (pkt_t*) hwapi_itf_get_pkt(x->hw_itf);
}
