#include "utilities.h"


void outb(uint16_t port, uint8_t val) {
    // Copied from http://wiki.osdev.org
    asm volatile("outb %0, %1" : : "a"(val), "Nd"(port));
}

uint8_t inb(uint16_t port) {
    // Copied from http://wiki.osdev.org
    uint8_t ret;
    asm volatile("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}
