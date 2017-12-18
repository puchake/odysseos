#!/bin/bash

AS=~/opt/cross/bin/i686-elf-as
GCC=~/opt/cross/bin/i686-elf-gcc

$AS boot.s -o obj/boot.o
$GCC -c kernel/main.c -o obj/main.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
$GCC -T linker.ld -o bin/os.bin -ffreestanding -O2 -nostdlib obj/* -lgcc

cp bin/os.bin isodir/boot/os.bin
grub-mkrescue -o iso/os.iso isodir
