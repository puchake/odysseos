#!/bin/bash

AS=~/opt/cross/bin/i686-elf-as
GCC=~/opt/cross/bin/i686-elf-gcc

$GCC -c kernel/pic.c -o obj/pic.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
$GCC -c kernel/utilities.c -o obj/utilities.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
$AS kernel/interrupts/interrupts.s -o obj/interrupts_s.o
$GCC -c kernel/interrupts/interrupts.c -o obj/interrupts_c.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
$AS kernel/initialize.s -o obj/initialize_s.o
$GCC -c kernel/initialize.c -o obj/initialize_c.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
$AS boot.s -o obj/boot.o
$GCC -c kernel/main.c -o obj/main.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
$GCC -T linker.ld -o bin/os.bin -ffreestanding -O2 -nostdlib obj/* -lgcc

cp bin/os.bin isodir/boot/os.bin
grub-mkrescue -o iso/os.iso isodir
