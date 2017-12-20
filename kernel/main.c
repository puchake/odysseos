#include <stdbool.h> 
#include <stddef.h>
#include <stdint.h>

#include "initialize.h"
#include "keyboard.h"
#include "terminal.h"
#include "disk.h"
#include "utilities.h"

void read_disk(const char * input_buffer) {
	unsigned char buffer[1024] = {0};
    char argument_buffer[256] = {0};
	int access_melo;
    unsigned char direction = ATA_READ;
    unsigned char drive = ATA_SECONDARY;
    unsigned int lba = 9584;
    unsigned char numsects = 2;
    unsigned short selector;
    zero_buffer(argument_buffer, 256);
    extract_word(input_buffer, argument_buffer, 256, 1);
    lba = atoi(argument_buffer);
	access_melo = ide_ata_access(direction, drive, lba, numsects, selector, buffer);


	for (int i = 0; i < 1024; i++) {
            write_character(buffer[i]);
        }
    write_string("\n");
}

typedef void (*func_ptr)(const char *) ;
typedef struct {
    func_ptr func;
    const char * name;
} command;

void kernel_main(void) {
    initialize_descriptors_tables();
    initialize_terminal();
    char lolo[30];
    char input_buffer[256] = {0};
    char command_buffer[256] = {0};

    command commands[] = {
        { read_disk, "readdisk" }
    };
    
    ide_initialize(0x1F0, 0x3F6, 0x170, 0x376, 0x000);

    while(1){
        zero_buffer(command_buffer, 256);
        write_string("> ");
        get_input(input_buffer, 256);
        extract_word(input_buffer, command_buffer, 256, 0);
        for (int i = 0; i < sizeof(commands)/sizeof(command); i++) {
            if (strcmp(commands[i].name, command_buffer)) {
                commands[i].func(input_buffer);
            }
        }
        
    };
}

