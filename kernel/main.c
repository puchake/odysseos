#include <stdbool.h> 
#include <stddef.h>
#include <stdint.h>

#include "initialize.h"
#include "keyboard.h"
#include "terminal.h"
#include "disk.h"

void kernel_main(void) {
    initialize();
    initialize_terminal();
    char lolo[30];
    char input_buffer[256] = {0};

    ide_initialize(0x1F0, 0x3F6, 0x170, 0x376, 0x000);

	unsigned char buffer[2048];
	int access_melo;
    unsigned char direction = ATA_READ;
    unsigned char drive = ATA_SECONDARY;
    unsigned int lba = 0;
    unsigned char numsects = 1;
    unsigned short selector;
	write_string("Reading a disk\n");

	access_melo = ide_ata_access(direction, drive, lba, numsects, selector, buffer);

	for (int i = 0; i < 32; i++) {
		write_string(itoas(buffer[i], lolo, 16));
		write_string(" ");
	}
    write_string("\n");

    while(1){
        write_string("> ");
        get_input(input_buffer, 256);
    };
}

