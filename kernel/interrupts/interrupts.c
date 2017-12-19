#include "interrupts.h"
#include "../pic.h"


void timer_handler() {
    uint16_t* terminal_buffer = (uint16_t*) 0xB8000;
    *(terminal_buffer + 1) = 'b';
    send_eoi(TIMER_IRQ);
}

void keystroke_handler() {
    uint16_t* terminal_buffer = (uint16_t*) 0xB8000;
    *terminal_buffer = 'a';
    send_eoi(KEYSTROKE_IRQ);
}

