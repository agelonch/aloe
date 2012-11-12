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

#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stddef.h>
#include <unistd.h>

#include "defs.h"
#include "str.h"
#include "hwapi.h"
#include "hwapi_error.h"

static lstrdef(tmp_path);
static strdef(syspath);

/**
 * Sets the real filesystem relative path where hwapi_file_open() operates.
 * @param path Real file-system path.
 */
int hwapi_file_set_path(string path) {
	HWAPI_ASSERT_PARAM(path);

	struct stat st;

	if(stat(path,&st)) {
		HWAPI_SYSERROR("stat");
		return -1;
	}

	strcpy(syspath, path);

	return 0;
}

/**
 * Given a pathname for a file, hwapi_file_open() returns a file descriptor,
 * a nonnegative integer for use in hwapi_file_write(), hwapi_file_read() and
 * hwapi_file_close().
 * The function access the file-system directory configured by a previous call to
 * hwapi_file_set_path().
 *
 * @param name Name of the file
 * @return nonnegative integer on success, -1 on error.
 */
int hwapi_file_open(string name) {
	HWAPI_ASSERT_PARAM(name);
	int fd;

	sprintf(tmp_path, "%s/%s", syspath, name);

	fd = open(tmp_path, O_RDWR, O_CREAT | O_NONBLOCK);
	if (fd == -1) {
		HWAPI_SYSERROR("open");
		return -1;
	}
	return fd;
}

/**
 * Closes a previously opened file using hwapi_file_open()
 * @param fd File descriptor returned by hwapi_file_open()
 * @return zero on success, -1 on error.
 */
int hwapi_file_close(int fd) {
	HWAPI_ASSERT_PARAM(fd>0);

	if (close(fd)) {
		HWAPI_SYSERROR("open");
		return -1;
	}
	return 0;
}

/**
 * Writes up to size bytes from the buffer pointed by buf to the
 * file referred to by the file descriptor fd.
 *
 * @param fd File descriptor returned by hwapi_file_open()
 * @param buf Pointer to the memory to write to the file
 * @param size Number of bytes to write
 * @return On success, the number of bytes written is returned (zero indicates
 * nothing was written). On error, -1 is returned
 */
int hwapi_file_write(int fd, void* buf, int size) {
	HWAPI_ASSERT_PARAM(fd>0);
	HWAPI_ASSERT_PARAM(buf);
	HWAPI_ASSERT_PARAM(size>0);

	int s;

	s = write(fd, buf, (size_t) size);
	if (s == -1) {
		HWAPI_SYSERROR("write");
		return -1;
	}

	return s;
}

/**
 * Attemps to read up to size bytes from the the file descriptor fd into
 * the buffer starting at buf.
 *
 * @param fd File descriptor returned by hwapi_file_open()
 * @param buf Pointer to the memory to write to the file
 * @param size Number of bytes to write
 * @return On success, the number of bytes written is returned (zero indicates
 * nothing was written). On error, -1 is returned
 */
int hwapi_file_read(int fd, void* buf, int size) {
	HWAPI_ASSERT_PARAM(fd>0);
	HWAPI_ASSERT_PARAM(buf);
	HWAPI_ASSERT_PARAM(size>0);

	int s;

	s = read(fd, buf, (size_t) size);
	if (s == -1) {
		HWAPI_SYSERROR("read");
		return -1;
	}

	return s;
}
