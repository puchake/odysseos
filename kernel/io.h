#ifndef IO_H_INCLUDED
#define IO_H_INCLUDED

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

void io_wait(void);

void outb(uint16_t port, uint8_t val);

void outw(uint16_t port, uint16_t val);

void outl(uint16_t port, uint32_t val);


uint8_t inb(uint16_t port);

uint16_t inw(uint16_t port);

uint32_t inl(uint16_t port);

bool are_interrupts_enabled();

void insl(uint16_t port, uint32_t * buffer, int quads);

void insw(uint16_t port, uint16_t * buffer, int words);

void insb(uint16_t port, uint8_t * buffer, int bytes);

#endif
