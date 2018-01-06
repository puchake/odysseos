#ifndef UTILITIES_H_INCLUDED
#define UTILITIES_H_INCLUDED

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

void reverse(char s[]);

void sleep(int time);

char * itoas(int n, char s[], int system);

void outb(uint16_t port, uint8_t val);

uint8_t inb(uint16_t port);

void outw(uint16_t port, uint16_t val);

void outl(uint16_t port, uint32_t val);

uint8_t inb(uint16_t port);

uint16_t inw(uint16_t port);

uint32_t inl(uint16_t port);

bool are_interrupts_enabled();

void insl(uint16_t port, uint32_t * buffer, int quads);

void insw(uint16_t port, uint16_t * buffer, int words);

void insb(uint16_t port, uint8_t * buffer, int bytes);

void outsl(uint16_t port, const uint32_t * buffer, int quads);

void outsw(uint16_t port, const uint16_t * buffer, int words);

void outsb(uint16_t port, const uint8_t * buffer, int bytes);

void set_memory(char byte, char* start, int bytes_count);

void copy_memory(char* source, char* destination, int bytes_count);

char to_lower(char character);

char to_upper(char character);

int strlen(const char* str);

char to_upper(char character);

bool is_printable(char character);

void extract_word(
    char* source_buffer, char* destination_buffer, 
    int destination_length, int word_i
);

int atoi(char* string);

void zero_buffer(char* buffer, int max_length);

bool strcmp(const char* string_1, const char* string_2);

#endif
