
SUBDIRS = 	loader \
			kernel \
	  		drivers \
	  		include 

EXTRA_DIST = 

image:
	$(MAKE) $(AM_MAKEFLAGS) all
	## create bootable floppy
	## standart floppy disk (2880 sectors) 512x2880=1440 kB
	dd if=/dev/zero of=floppy.img bs=512 count=2880 2>/dev/null
	## copy boot loader to 1-st sector of disk
	dd if=loader/loader of=floppy.img bs=512 count=1 conv=notrunc 2>/dev/null
	## copy the kernel image starting 1-st sector of floppy
	dd if=kernel/kernel of=floppy.img bs=512 seek=1 conv=notrunc 2>/dev/null