/*
 * This file is subject to the terms and conditions of the GNU General Public
 * License. See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (C) 2013 Álvaro Fernández Rojas <noltari@gmail.com>
 */

#include <fcntl.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
 
#include "atheros_lib.h"
#include "atheros_extract.h"

#define pr_fmt(fmt) "athext: " fmt

int main (int argc, char **argv) {
	char *file_input = NULL, *file_output = NULL;
	int eeprom_offset = -1, eeprom_size = ATHEROS_EEPROM_SIZE;
	bool help = false;

	// Parse arguments
	while(true) {
		int this_option_optind = optind ? optind : 1;
		int option_index = 0;
		static struct option long_options[] = {
			{"help", no_argument, NULL, 0},
			{"input", required_argument, NULL, 0},
			{"output", required_argument, NULL, 0},
			{"offset", required_argument, NULL, 0},
			{"size", required_argument, NULL, 0},
			{NULL, no_argument, NULL, 0}
		};

		int c = getopt_long_only(argc, argv, "", long_options, &option_index);
		if(c == 0) {
			switch(option_index) {
				case OPT_HELP:
					help = true;
					break;
				case OPT_INPUT:
					file_input = optarg;
					break;
				case OPT_OUTPUT:
					file_output = optarg;
					break;
				case OPT_OFFSET:
					eeprom_offset = atoi(optarg);
					break;
				case OPT_SIZE:
					eeprom_size = atoi(optarg);
					break;
			}
		}
		else {
			break;
		}
	}

	// Check if needed arguments have been supplied
	if(file_input == NULL || file_output == NULL || eeprom_offset < 0) {
		help = true;
	}

	if(help) {
		// Show help
		printf("%s\n", argv[0]);
		printf("\tinput: file where the eeprom is read from\n");
		printf("\toutput: file where the eeprom is stored\n");
		printf("\toffset: offset of the eeprom (>0)\n");
		printf("\t<size>: size of the eeprom [%d]\n", ATHEROS_EEPROM_SIZE);
	}
	else {
		uint8_t bytes[eeprom_size];
		int fdin, fdout, bytes_read, bytes_written;

		// Open input file
		fdin = open(file_input, O_RDONLY, S_IRWXU | S_IRWXG | S_IRWXO);
		printf("Input: %s\n", file_input);
		printf("Offset: 0x%x (%d)\n", eeprom_offset, eeprom_offset);
		printf("Size: 0x%x (%d)\n", eeprom_size, eeprom_size);

		// Read eeprom
		bytes_read = athlib_bytes_read(fdin, (uint8_t*) bytes, eeprom_offset, eeprom_size);
		if(bytes_read == eeprom_size) {
			// Open output file
			fdout = open(file_output, O_CREAT|O_WRONLY|O_TRUNC, S_IRWXU + S_IRWXG + S_IRWXO);
			printf("Output: %s\n", file_output);

			// Write eeprom
			bytes_written = athlib_bytes_write(fdout, (uint8_t*) bytes, 0, eeprom_size, true);
			if(bytes_written == eeprom_size) {
				printf("Result: success.\n", file_output);
			}
			else {
				// Error while writing eeprom
				printf("Result: write error.\n", file_output);
			}
		}
		else {
			// Error while reading eeprom
			printf("Result: read error.\n", file_output);
		}

		// Close files
		close(fdin);
		close(fdout);
	}

	exit(EXIT_SUCCESS);
}
