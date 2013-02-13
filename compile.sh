#!/bin/sh

## compile boot loader
echo -n "Building the bootloader.."
gcc -I./include -D__GAS__ -c loader/boot.S
ld -T loader/mbr.ld -o loader.img boot.o

if [ $? -eq 0 ] 
then 
	## clean 
	rm *.o
	echo "done"
else
	exit 1
fi

## compile the kernel
echo -n "Building the kernel.."
gcc -m32 -I./include -D__GAS__ -c kernel/i386/start.S
gcc -m32 -I./include -D__GAS__ -c kernel/i386/version.S
gcc -m32 -I./include -D__GAS__ -c kernel/i386/protmode.S
gcc -m32 -I./include -D__GAS__ -c kernel/i486/cpu.S
gcc -m32 -I./include -fmessage-length=0 -Wall -g0 -O0 -fno-inline -c kernel/kmain.c
gcc -m32 -I./include -fmessage-length=0 -Wall -g0 -O0 -fno-inline -c kernel/console.c
gcc -m32 -I./include -fmessage-length=0 -Wall -g0 -O0 -fno-inline -c kernel/cpuinfo.c
gcc -m32 -I./include -fmessage-length=0 -w -g0 -O0 -fno-inline -c kernel/std/membase.c
ld -mi386linux -M -T kernel/link.ld -o kernel.img *.o 1>linkage.map

if [ $? -eq 0 ] 
then 
	## clean 
	rm *.o
	echo "done"
else
	exit 1
fi

## create floppy
echo -n "Creating booting floppy image.."
## standart floppy disk (2880 sectors) 512x2880=1440 kB
dd if=/dev/zero of=floppy.img bs=512 count=2880 2>/dev/null
## copy boot loader to 1-st sector of disk
dd if=loader.img of=floppy.img bs=512 count=1 conv=notrunc 2>/dev/null
## copy the kernel image starting 1-st sector of floppy
dd if=kernel.img of=floppy.img bs=512 seek=1 conv=notrunc 2>/dev/null
echo "done"

exit 0

