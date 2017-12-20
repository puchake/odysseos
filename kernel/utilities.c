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


void set_memory(char byte, char* start, int bytes_count) {
    while (bytes_count != 0) {
        *start = byte;
        start++;
        bytes_count--;
    }
}


void copy_memory(char* source, char* destination, int bytes_count) {
    while (bytes_count != 0) {
        *destination = *source;
        source++;
        destination++;
        bytes_count--;
    }
}


char to_lower(char character) {
    if (character >= 'A' && character <= 'Z') {
        return character + ('a' - 'A');
    }
    else {
        return character;
    }
}


char to_upper(char character) {
    if (character >= 'a' && character <= 'z') {
        return character - ('a' - 'A');
    }
    else {
        return character;
    }
}


int strlen(const char* str) {
    int len = 0;
    while (str[len]) {
	len++;
    }
    return len;
}

