#include <stdbool.h> 
#include <stddef.h>
#include <stdint.h>

#include "initialize.h"
#include "keyboard.h"
#include "terminal.h"
#include "utilities.h"
#include "disk.h"
#include "commands.h"

void kernel_main(void) {
    initialize_descriptors_tables();
    initialize_terminal();
    if (initialize_keyboard_controller()) {
	write_string("Keyboard controller has been initialized.\n");
    }
    else {
	write_string("Error in keyboard controller initialization.\n");
    }
    char input_buffer[256] = {0};
    char command_buffer[256] = {0};

    command commands[] = {
        { read_disk, "readdisk" },
        { write_disk, "writedisk" },
        { print_keyboard_state, "print_keyboard_state" },
        { get_scancodes_set_command, "get_scancodes_set" },
        { set_scancodes_set_command, "set_scancodes_set" }
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

