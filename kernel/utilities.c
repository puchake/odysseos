#include "utilities.h"

extern volatile int timer_i;
void sleep(int time) {
    int s_time = timer_i + time;
    while (timer_i < s_time);
}

extern inline void outb(uint16_t port, uint8_t val)
{
    // Copied from http://wiki.osdev.org
  asm volatile ( "outb %0, %1" : : "a"(val), "Nd"(port) );
}

extern inline void outw(uint16_t port, uint16_t val)
{
  asm volatile ( "outw %0, %1" : : "a"(val), "d"(port) );
}

extern inline void outl(uint16_t port, uint32_t val)
{
  asm volatile ( "outl %0, %1" : : "a"(val), "d"(port) );
}


extern inline uint8_t inb(uint16_t port)
{
    // Copied from http://wiki.osdev.org
  uint8_t ret;
  asm volatile ( "inb %1, %0"
          : "=a"(ret)
          : "Nd"(port) );
  return ret;
}

extern inline uint16_t inw(uint16_t port)
{
  uint16_t ret;
  asm volatile ( "inw %1, %0"
          : "=a"(ret)
          : "d"(port) );
  return ret;
}

extern inline uint32_t inl(uint16_t port)
{
  uint32_t ret;
  asm volatile ( "inl %1, %0"
          : "=a"(ret)
          : "d"(port) );
  return ret;
}

extern inline void insl(uint16_t port, uint32_t * buffer, int quads) {
    asm("rep insl" : : "c"(quads), "d"(port), "D"(buffer));
}

extern inline void insw(uint16_t port, uint16_t * buffer, int words) {
    asm("rep insw" : : "c"(words), "d"(port), "D"(buffer));
}

extern inline void insb(uint16_t port, uint8_t * buffer, int bytes) {
    asm("rep insb" : : "c"(bytes), "d"(port), "D"(buffer));
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

