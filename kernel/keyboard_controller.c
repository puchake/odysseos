#include "keyboard_controller.h"
#include "utilities.h"
#include <stdint.h>


struct KeyboardState get_keyboard_state() {
    char state = inb(KEYBOARD_STATUS_PORT);
    struct KeyboardState keyboard_state = {
        .output_status = state & 0x01 > 0, .input_status = state & 0x02 > 0,
        .system_status = state & 0x04 > 0, .a2_state = state & 0x08 > 0,
 	.inhibit_flag = state & 0x10 > 0, .mouse_output_status = state & 0x20 > 0,
	.timeout_flag = state & 0x40 > 0, .parity_flag = state & 0x80 > 0
    };
    return keyboard_state;
}


bool is_output_full() {
    char state = inb(KEYBOARD_STATUS_PORT);
    return state & 0x01 > 0;
}


bool is_input_full() {
    char state = inb(KEYBOARD_STATUS_PORT);
    return state & 0x02 > 0;
}


bool timeout_occured() {
    char state = inb(KEYBOARD_STATUS_PORT);
    return state & 0x40 > 0;
}


char send_command_to_keyboard_controller(char command) {
    outb(KEYBOARD_STATUS_PORT, command);
    // Wait for controller response which does not go through interrupt.
    while(!is_output_full());
    return inb(KEYBOARD_DATA_PORT);
}


void send_byte_to_keyboard(char byte) {
    outb(KEYBOARD_DATA_PORT, byte);
    // Wait for keyboard to get the command.
    while(is_input_full());
}


char get_byte_from_keyboard() {
    while(!is_output_full());
    return inb(KEYBOARD_DATA_PORT);
}


bool initialize_keyboard_controller() {
    // Disable both PS2 ports.
    outb(KEYBOARD_STATUS_PORT, DISABLE_FIRST_PS2_PORT);
    outb(KEYBOARD_STATUS_PORT, DISABLE_SECOND_PS2_PORT);
    // Flush port 0x60.
    inb(KEYBOARD_DATA_PORT);
    char test_result = send_command_to_keyboard_controller(TEST_CONTROLLER);
    // Enable both PS2 ports.
    outb(KEYBOARD_STATUS_PORT, ENABLE_FIRST_PS2_PORT);
    outb(KEYBOARD_STATUS_PORT, ENABLE_SECOND_PS2_PORT);
    return test_result == CONTROLLER_TEST_OK;
}
