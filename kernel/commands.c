#include "disk.h"
#include "terminal.h"
#include "utilities.h"
#include "keyboard.h"
#include "keyboard_controller.h"


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


void print_keyboard_state(const char * input_buffer) {
    write_string("Keyboard status:\n");
    struct KeyboardState state = get_keyboard_state();
    write_string("Output buffer status: ");
    if (state.output_status == OUTPUT_BUFFER_EMPTY) {
	write_string("Empty\n");
    }
    else {
        write_string("Full\n");
    }
    write_string("Input buffer status: ");
    if (state.input_status == INPUT_BUFFER_EMPTY) {
	write_string("Empty\n");
    }
    else {
        write_string("Full\n");
    }
    write_string("System status: ");
    if (state.system_status == SYSTEM_POWER_UP) {
	write_string("Power up\n");
    }
    else {
        write_string("Initialized\n");
    }
    write_string("A2 line status: ");
    if (state.a2_state == A2_PORT_0X60) {
	write_string("Port 0x60 was last written to.\n");
    }
    else {
        write_string("Port 0x64 was last written to.\n");
    }
    write_string("Keyboard inhibit flag: ");
    if (state.inhibit_flag == KEYBOARD_INHIBITED) {
	write_string("Keyboard is inhibited.\n");
    }
    else {
        write_string("Keyboard is active.\n");
    }
    write_string("Mouse output buffer status: ");
    if (state.mouse_output_status == OUTPUT_BUFFER_EMPTY) {
	write_string("Empty\n");
    }
    else {
        write_string("Full\n");
    }
    write_string("Timeout flag: ");
    if (state.timeout_flag == NO_KEYBOARD_TIMEOUT) {
	write_string("No error\n");
    }
    else {
        write_string("Error\n");
    }
    write_string("Parity flag: ");
    if (state.parity_flag == NO_PARITY_ERROR) {
	write_string("No error\n");
    }
    else {
        write_string("Error\n");
    }
}

void get_scancodes_set_command(const char * input_buffer) {
    write_string("Active scancodes set is: ");
    char set_number = get_scancodes_set();
    write_character(set_number);
    write_string("\n");
}


void set_scancodes_set_command(const char * input_buffer) {
    char argument_buffer[256] = {0};
    char set_number;
    extract_word(input_buffer, argument_buffer, 256, 1);
    set_number = atoi(argument_buffer);
    if (set_scancodes_set(set_number)) {
        write_string("Scancodes set changed successfully.\n");
    }
    else {
        write_string("Scancodes set change failed.\n");
    }
}


void write_disk(const char * input_buffer) {
    unsigned char buffer[1024] = {0};
    char argument_buffer[256] = {0};
    int access;
    unsigned char direction = ATA_READ;
    unsigned char drive = ATA_SECONDARY;
    unsigned int lba = 9584;
    unsigned int byte_index = 436;
    unsigned char numsects = 1;
    unsigned short selector;
    
    zero_buffer(argument_buffer, 256);
    extract_word(input_buffer, argument_buffer, 256, 1);
    lba = atoi(argument_buffer);

    zero_buffer(argument_buffer, 256);
    extract_word(input_buffer, argument_buffer, 256, 2);
    byte_index = atoi(argument_buffer);

    zero_buffer(argument_buffer, 256);
    extract_word(input_buffer, argument_buffer, 256, 3);

    direction = ATA_READ;
    drive = ATA_SECONDARY;
    numsects = 1;
    access = ide_ata_access(direction, drive, lba, numsects, selector, buffer);

    write_string("\nBefore:\n");
    for (int i = 0; i < 512; i++) {
        write_character(buffer[i]);
    }

    //w buforze mamy teraz przeczytany sektor

    buffer[byte_index] = argument_buffer[0];

    write_string("\nAfter:\n");
    for (int i = 0; i < 512; i++) {
        write_character(buffer[i]);
    }

    direction = ATA_WRITE;
    drive = ATA_SECONDARY;
    numsects = 1;
    access = ide_ata_access(direction, drive, lba, numsects, selector, buffer);
    

    write_string("\n");
}
