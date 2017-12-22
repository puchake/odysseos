#include <stdbool.h>
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

void reverse(char s[])
{
    int i, j;
    char c;

    for (i = 0, j = strlen(s)-1; i<j; i++, j--) {
	 c = s[i];
	 s[i] = s[j];
	 s[j] = c;
    }
}

char * itoas(int n, char s[], int system) {
     //converts integer into a string in specified system (e.g. hex or dec)
     int i, sign;
     char signs[16] = "0123456789ABCDEF";
 
     if ((sign = n) < 0)  /* record sign */
         n = -n;          /* make n positive */
     i = 0;
     do {       /* generate digits in reverse order */
         s[i++] = signs[n % system]; // + '0';   /* get next digit */
     } while ((n /= system) > 0);     /* delete it */
     if (sign < 0)
         s[i++] = '-';
     s[i] = '\0';
     reverse(s);

     return s;
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

bool is_printable(char character) {
    return (character >= ' ' && character <= '~') 
           || character == '\b' 
           || character == '\n';
}

void extract_word(
    char* source_buffer, char* destination_buffer, 
    int destination_length, int word_i
) {
    while (word_i != 0 && *source_buffer != 0) {
        if (*source_buffer == ' ') {
            word_i--;
        }
        source_buffer++;
    }
    while (*source_buffer != 0 && *source_buffer != ' ') {
        *destination_buffer = *source_buffer;
        destination_buffer++;
        source_buffer++;
    }
}

int atoi(char* string) {
    int result = 0;
    while (*string != 0) {
        result *= 10;
        result += *string - '0';
        string++;
    }
    return result;
}

void zero_buffer(char* buffer, int max_length) {
    for (int i = 0; i < max_length; i++) {
        buffer[i] = 0;
    }
}

bool strcmp(const char* string_1, const char* string_2) {
    while (*string_1 != 0 && *string_2 != 0) {
        if (*string_1 != *string_2) {
            return false;
        }
        string_1++;
        string_2++;
    }
    return *string_1 == *string_2;
}
