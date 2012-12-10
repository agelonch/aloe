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
#include "str.h"
#include "rtdal_itf.h"
#include "rtdal_itfspscq.h"
#include "rtdal_itfphysic.h"
#include "rtdal.h"

#define call(a, ...) if (obj->is_external) return rtdal_itfphysic_##a(__VA_ARGS__); else return rtdal_itfspscq_##a(__VA_ARGS__)

int rtdal_itf_remove(r_itf_t obj) {
	call(remove,obj);
}

/** Receives up to len bytes from the interface and stores the data in the
 * memory pointed by buffer.
 *
 * \param obj Handler returned by rtdal_itfspscq_create()
 * \param buffer Pointer to the buffer to store the received data
 * \param len Length of the buffer in bytes
 *
 * \returns A non-negative number indicating the number of useful received bytes,
 * 0 if there are no pending packets in the interface or -1 on error
 */
int rtdal_itf_recv(r_itf_t obj, void* buffer, int len) {
	call(recv,obj,buffer,len);
}

/** Sends len bytes from the memory pointed by buffer through the interface
 * After rtdal_itfspscq_send() returns, the user may reuse the buffer.
 *
 * \param obj Handler returned by rtdal_itfspscq_create()
 * \param buffer Pointer to the data to transmit
 * \param len Number of bytes to transmit
 *
 * \returns 1 if the packet was sent, 0 if there is no space in the interface or -1 on error
 */
int rtdal_itf_send(r_itf_t obj, void* buffer, int len) {
	call(send,obj,buffer,len);
}

int rtdal_itf_set_blocking(r_itf_t obj, int block) {
	call(set_blocking,obj,block);
}

int rtdal_itf_get_blocking(r_itf_t obj) {
	call(get_blocking,obj);
}

/**Saves in ptr the address of the buffer that will be transmitted in a subsequent call
 * to rtdal_itfspscq_push(). Two consecutive calls to rtdal_itfspscq_request() return the same
 * address until rtdal_itfspscq_push() is called.
 *
 * \param obj Handler returned by rtdal_itfspscq_create()
 * \param ptr Address of the pointer
 * \returns 1 on success, 0 if there are no free packets in the interface or -1 on error
 */
int rtdal_itf_request(r_itf_t obj, void **ptr) {
	call(request,obj,ptr);
}

/**Releases the packet received by rtdal_itfspscq_pop() after the contents have been
 * processed. The buffer then will be reused by the producer.
 *
 * \param obj Handler returned by rtdal_itfspscq_create()
 *
 * \returns 1 on success, 0 if there are no packets to release in the interface or -1 on error
 */
int rtdal_itf_release(r_itf_t obj) {
	call(release,obj);
}

/**Pushes the current packet to the interface. The contents of the packet are modified using
 * the pointer returned by rtdal_itfspscq_request().
 *
 * \param obj Handler returned by rtdal_itfspscq_create()
 * \param len Number of useful bytes in the packet.
 * \returns 1 on success, 0 if there is no space in the interface or -1 on error
 */
int rtdal_itf_push(r_itf_t obj, int len) {
	call(push,obj,len);
}


/**Pops a packet from the interface. The address of the packet contents are saved in the
 * ptr pointer and the useful number of bytes is saved in len.
 * A packet will be received if the timestamp at transmission plus the interface delay is lower
 * or equal to the current timestamp.
 *
 * \param obj Handler returned by rtdal_itfspscq_create()
 * \param ptr Address of the pointer
 * \param len Number of useful bytes in the packet.
 *
 * \returns 1 on success, 0 if there are no packets pending in the interface or -1 on error
 */
int rtdal_itf_pop(r_itf_t obj, void **ptr, int *len) {
	call(pop,obj,ptr,len);
}

int rtdal_itf_set_callback(r_itf_t obj, void (*fnc)(void), int prio) {
	call(set_callback,obj,fnc,prio);
}

int rtdal_itf_set_delay(r_itf_t obj, int delay) {
	call(set_delay,obj,delay);
}

int rtdal_itf_get_delay(r_itf_t obj) {
	call(get_delay,obj);
}

