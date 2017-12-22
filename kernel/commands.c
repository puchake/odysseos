#include "disk.h"
#include "terminal.h"
#include "utilities.h"

void read_disk(const char * input_buffer) {
	unsigned char buffer[1024] = {0};
    char argument_buffer[256] = {0};
	int access;
    unsigned char direction = ATA_READ;
    unsigned char drive = ATA_SECONDARY;
    unsigned int lba = 9584;
    unsigned char numsects = 2;
    unsigned short selector;
    zero_buffer(argument_buffer, 256);
    extract_word(input_buffer, argument_buffer, 256, 1);
    lba = atoi(argument_buffer);
    access = ide_ata_access(direction, drive, lba, numsects, selector, buffer);
    for (int i = 0; i < 512; i++) {
        write_character(buffer[i]);
    }
    write_string("\n");
}
