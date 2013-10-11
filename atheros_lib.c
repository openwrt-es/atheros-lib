/*
 * This file is subject to the terms and conditions of the GNU General Public
 * License. See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (C) 2013 Álvaro Fernández Rojas <noltari@gmail.com>
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "atheros_lib.h"

#define pr_fmt(fmt) "athlib: " fmt

/*
 * Read bytes from file.
 * Fails if the size supplied isn't fully read.
 */
int athlib_bytes_read(int fdin, uint8_t* bytes, int bytes_off, int bytes_len) {
	int file_len;
	struct stat st;

	// Check file descriptor
	if(!fdin) {
		pr_err("file descriptor error.\n");
		return 0;
	}

	// Check if file exists
	if(fstat(fdin, &st) == -1) {
		pr_err("file doesn't exist.\n");
		return 0;
	}

	// Check if file isn't empty
	file_len = (int) st.st_size;
	if(file_len == 0) {
		pr_err("file is empty.\n");
		return 0;
	}

	// Check offset
	if(bytes_off >= file_len) {
		pr_err("wrong offset.\n");
		return 0;
	}

	// Check size
	if(bytes_len >= file_len || bytes_off + bytes_len > file_len) {
		pr_err("wrong size.\n");
		return 0;
	}

	// Seek file
	if(bytes_off > 0 && lseek(fdin, bytes_off, SEEK_SET) < 0) {
		pr_err("error while seeking.\n");
		return 0;
	}

	// Read the file
	ssize_t bytes_read = read(fdin, bytes, bytes_len);
	if(bytes_read != bytes_len) {
		pr_err("file wasn't read properly.\n");
		return 0;
	}

	return bytes_read;
}
/*
 * Writes bytes to file.
 * Fails if the size supplied isn't fully written.
 */
int athlib_bytes_write(int fdout, uint8_t* bytes, int bytes_off, int bytes_len, bool new) {
	// Check file descriptor
	if(!fdout) {
		pr_err("file descriptor error.\n");
		return 0;
	}

	// Check if file has to be created.
	if(!new && bytes_off > 0) {
		struct stat st;

		// Check if file exists
		if(fstat(fdout, &st) == -1) {
			pr_err("file doesn't exist.\n");
			return 0;
		}

		// Seek file
		if(lseek(fdout, bytes_off, SEEK_SET) < 0) {
			pr_err("error while seeking.\n");
			return 0;
		}
	}

	// Write the file
	ssize_t bytes_written = write(fdout, bytes, bytes_len);
	if(bytes_written != bytes_len) {
		pr_err("file wasn't written properly.\n");
		return 0;
	}

	return bytes_written;
}
/*
 * Prints an array of bytes.
 */
int athlib_bytes_print(uint8_t* bytes, int size) {
	int i;

	// Print char by char
	for(i = 0; i < size; i++) {
		printf("%c", bytes[i]);
	}
	printf("\n");

	return i;
}
