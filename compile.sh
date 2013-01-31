#!/bin/sh

## compile boot loader
gcc -I./include -D__GAS__ -c loader/boot.S
ld -T loader/mbr.ld -o loader.img boot.o

## compile the kernel
gcc -I./include -D__GAS__ -c kernel/i386/start.S
ld -T kernel/link.ld -o kernel.img start.o

## create floppy
## standart floppy disk (2880 sectors) 512x2880=1440 kB
dd if=/dev/zero of=floppy.img bs=512 count=2880
## copy boot loader to 1-st sector of disk
dd if=loader.img of=floppy.img bs=512 count=1 conv=notrunc
## copy the kernel image starting 1-st sector of floppy
dd if=kernel.img of=floppy.img bs=512 seek=1 count=127 conv=notrunc

exit 0

