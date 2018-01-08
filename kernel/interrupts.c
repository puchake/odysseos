#include "interrupts.h"
#include "pic.h"
#include "keyboard.h"
#include "utilities.h"


volatile int timer_i = 0;


void timer_handler() {
    timer_i++;
    send_eoi(TIMER_IRQ);
}


void keystroke_handler() {;
    if (keyboard_input_state == WAITING_FOR_KEYBOARD_INPUT) {
        // Fill in the scancode buffer, if get_char is waiting for input.
        char scancode_byte = 0;
        char next_scancode_byte = inb(KEYBOARD_IN_PORT);
        int buffer_i = 0;
        zero_buffer(scancode_buffer, MAX_SCANCODE_LENGTH);
        do {
            scancode_buffer[buffer_i] = next_scancode_byte; 
            buffer_i += 1;           
	    scancode_byte = next_scancode_byte;
            next_scancode_byte = inb(KEYBOARD_IN_PORT);  
        }
        while (next_scancode_byte != scancode_byte); 
        // Switch buffer and input states to signal that the keystroke has
        // occured.
        scancode_buffer_state = SCANCODE_BUFFER_FULL;
        keyboard_input_state = IGNORE_KEYBOARD_INPUT; 
    }
    else if (keyboard_input_state == WAITING_FOR_KEYBOARD_RESPONSE) {
	// Handle keyboard command response.
        scancode_buffer[0] = inb(KEYBOARD_IN_PORT);
        keyboard_input_state = IGNORE_KEYBOARD_INPUT; 
    }
    else {
        // If nothing is waiting for input, simply read one byte from
        // keyboard's port to prevent keyboard's interrupt from locking.
        inb(KEYBOARD_IN_PORT);
    }
    send_eoi(KEYSTROKE_IRQ);
}

