AM_CPPFLAGS = -m32 -Wall -fmessage-length=0 -fno-strict-aliasing -fno-inline 
AM_CCASFLAGS = -D__GAS__
AM_LDFLAGS = -mi386linux -M -T link.ld

# default CFLAGS
CFLAGS= -g -O0


LINK=@LINKER@ $(AM_LDFLAGS) $(LDFLAGS) -o kernel 1>linkage.map

noinst_PROGRAMS = kernel

kernel_LDADD = $(top_builddir)/drivers/libdriver_def.a
kernel_SOURCES = i386/int.c \
				 i386/inthandlers.S \
				 i386/port.S \
				 i386/protmode.S \
				 i386/start.S \
				 i386/stop.S \
				 i386/version.S \
				 i486/cpu.S \
				 std/membase.c \
				 std/print.c \
				 std/strings.c \
				 console.c \
				 cpuinfo.c \
				 kerror.c \
				 kmain.c \
				 kmalloc.c \
				 mempool.c \
				 pic.c \
				 screen.c \
				 driver.c

EXTRA_DIST = link.ld