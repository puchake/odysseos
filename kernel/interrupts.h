// Offsets to which interrupts from PIC will be remapped.
#define MASTER_PIC_OFFSET	0x20
#define SLAVE_PIC_OFFSET	0x28

#define TIMER_IRQ 		0
#define KEYSTROKE_IRQ 		1

#include <stdint.h>


extern void timer_wrapper();
extern void keystroke_wrapper();

void timer_handler();
void keystroke_handler();
