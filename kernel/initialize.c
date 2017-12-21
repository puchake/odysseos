#include "initialize.h"
#include "interrupts.h"
#include "pic.h"


uint64_t create_gdt_descriptor(uint32_t base, uint32_t limit, uint16_t flag) {
    // Copied from http://wiki.osdev.org/GDT_Tutorial 
    uint64_t descriptor;
    // Create the high 32 bit segment.
    // Place limit bits 19:16 on the 51:48 bits of the descriptor.
    descriptor =  limit & 0x000F0000;
    // Place type, p, dpl, s, g, d/b, l and avl bits on 23:20 and 15:8 bits of
    // the descriptor.
    descriptor |= (flag << 8) & 0x00F0FF00;      
    // Set bits 39:32 as 23:16 bits of the base.
    descriptor |= (base >> 16) & 0x000000FF;
    // Set bits 63:56 as 31:24 bits of the base.
    descriptor |=  base & 0xFF000000;
    // Shift by 32 to allow for low part of segment.
    descriptor <<= 32;
    // Create the low 32 bit segment.
    // Set bits 31:16 as 15:0 bits of the base.
    descriptor |= base << 16;  
    // Set bits 15:0 as 15:0 buts of the limit.            
    descriptor |= limit & 0x0000FFFF;
    return descriptor;
}


uint64_t create_idt_interrupt_descriptor(uint32_t offset, uint32_t selector) {
    uint64_t descriptor;
    // Create the high 32 bit segment.
    // Set bits 63:48 as 31:16 bits of the offset.
    descriptor = offset & 0xFFFF0000;
    descriptor |= 0x00008E00; 
    // Shift by 32 to allow for low part of segment.
    descriptor <<= 32;
    // Create the low 32 bit segment
    // Set 31:16 bits as 15:0 bits of the selector.
    descriptor |= selector  << 16;                    
    // Set 15:0 bits as 15:0 bits of the offset.   
    descriptor |= offset  & 0x0000FFFF;               
    return descriptor;
}


void initialize_gdt(uint32_t gdt_offset) {
    // Create necessary descriptors for kernel: null, code and data.
    uint64_t null_descriptor = create_gdt_descriptor(0, 0, 0);
    uint64_t kernel_code_descriptor = create_gdt_descriptor(
        0, 0x000FFFFF, (GDT_CODE_PL0)
    );
    uint64_t kernel_data_descriptor = create_gdt_descriptor(
        0, 0x000FFFFF, (GDT_DATA_PL0)
    );
    // We don't have to block interrupts as these should already be blocked.
    uint64_t* gdtr = (uint64_t*) gdt_offset;
    *gdtr = null_descriptor;
    *(gdtr + 1) = kernel_code_descriptor;
    *(gdtr + 2) = kernel_data_descriptor;
    // Load GDTR with desired offset and limit equal to 3 * 8 - 1.
    set_gdtr(gdt_offset, 23);
    // reload_segments places 0x08 in CS and 0x10 in DS, so order of 
    // kernel code and data descriptors should not be changed.
    reload_segments();
}


void initialize_idt(uint32_t idt_offset) {
    // Create descriptors for wrappers in initialize.h
    uint64_t timer_descriptor = create_idt_interrupt_descriptor(
        (intptr_t) timer_wrapper, CODE_SELECTOR
    );
    uint64_t keystroke_descriptor = create_idt_interrupt_descriptor(
        (intptr_t) keystroke_wrapper, CODE_SELECTOR
    );
    // Fill IDT with them.
    uint64_t* idtr = (uint64_t*) idt_offset;
    *(idtr + MASTER_PIC_OFFSET + TIMER_IRQ) = timer_descriptor;
    *(idtr + MASTER_PIC_OFFSET + KEYSTROKE_IRQ) = keystroke_descriptor;
    // 2047 IDT limit equals to 256 interrupts.
    set_idtr(idt_offset, 2047);
    remap_pic(MASTER_PIC_OFFSET, SLAVE_PIC_OFFSET);
}


void initialize_descriptors_tables() {
    asm("cli");
    initialize_gdt(GDT_OFFSET);
    initialize_idt(IDT_OFFSET);
    asm("sti");
}


