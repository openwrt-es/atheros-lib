/*
 * This file is subject to the terms and conditions of the GNU General Public
 * License. See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (C) 2013 Álvaro Fernández Rojas <noltari@gmail.com>
 */

#ifndef ATHEROS_LIB_H
#define ATHEROS_LIB_H

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#define ATHEROS_EEPROM_SIZE	4096

#define pr_info(fmt, ...) fprintf(stdout, pr_fmt(fmt), ##__VA_ARGS__)
#define pr_err(fmt, ...) fprintf(stderr, pr_fmt(fmt), ##__VA_ARGS__)

int athlib_bytes_read(int fdin, uint8_t* bytes, int bytes_off, int bytes_len);
int athlib_bytes_write(int fdout, uint8_t* bytes, int bytes_off, int bytes_len, bool new);
int athlib_bytes_print(uint8_t* bytes, int size);

#endif /* ATHEROS_LIB_H */
