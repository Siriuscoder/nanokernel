#!/bin/sh

## compile boot loader
gcc -I./include -c loader/boot.S
ld -T loader/mbr.ld -o loader.img boot.o

## create floppy
## standart floppy disk (2880 sectors) 512x2880=1440 kB
dd if=/dev/zero of=floppy.img bs=512 count=2880
## copy boot loader to 1-st sector of disk
dd if=loader.img of=floppy.img bs=512 count=1 conv=notrunc

exit 0

