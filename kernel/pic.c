#include "pic.h"
#include "utilities.h"


void send_eoi(unsigned char irq) {
    // If interrupts comes from slave, send it EOI too.
    if (irq >= FIRST_SLAVE_IRQ) {
        outb(PIC2_COMMAND, PIC_EOI);
    }
    outb(PIC1_COMMAND, PIC_EOI);
}


void io_wait() {
     int i = 0;
     while (i != 10000) {i += 1;};
     int a = i + 10;
}


void remap_pic(unsigned char master_offset, unsigned char slave_offset) {
    unsigned char master_mask, slave_mask;
    // Save masks.
    master_mask = inb(PIC1_DATA);                    
    slave_mask = inb(PIC2_DATA);
    // Start initialization sequence.
    outb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);
    io_wait();  
    outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);
    io_wait();  
    outb(PIC1_DATA, master_offset);    
    io_wait();  
    outb(PIC2_DATA, slave_offset);   
    io_wait();  
    // ICW3: tell Master PIC that there is a slave PIC at IRQ2 (0000 0100)  
    outb(PIC1_DATA, 0x04); 
    io_wait();  
    // ICW3: tell Slave PIC its cascade identity (0000 0010)
    outb(PIC2_DATA, 0x02);   
    io_wait();    
    outb(PIC1_DATA, ICW4_8086);
    io_wait();  
    outb(PIC2_DATA, ICW4_8086);
    io_wait();  
    // Restore saved masks.
    outb(PIC1_DATA, master_mask); 
    io_wait();    
    outb(PIC2_DATA, slave_mask);
    io_wait();  
}

