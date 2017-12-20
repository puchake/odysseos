#include "interrupts.h"
#include "../pic.h"
#include "../keyboard.h"
#include "../utilities.h"


volatile int timer_i = 0;
void timer_handler() {
    timer_i++;
    send_eoi(TIMER_IRQ);
}


void keystroke_handler() {;
    if (keyboard_input_state == WAITING_FOR_KEYBOARD_INPUT) {
        char scancode_byte = 0;
        char next_scancode_byte = inb(KEYBOARD_IN_PORT);
        int buffer_i = 0;
        for (int i = 0; i < MAX_SCANCODE_LENGTH; i++) {
            scancode_buffer[i] = 0;
        }
        do {
            scancode_buffer[buffer_i] = next_scancode_byte; 
            buffer_i += 1;           
	    scancode_byte = next_scancode_byte;
            next_scancode_byte = inb(KEYBOARD_IN_PORT);  
        }
        while (next_scancode_byte != scancode_byte); 
        scancode_buffer_state = SCANCODE_BUFFER_FULL;
        keyboard_input_state = IGNORE_KEYBOARD_INPUT; 
    }
    send_eoi(KEYSTROKE_IRQ);
}

