AM_CPPFLAGS = -m32 -Wall -fmessage-length=0
CCASFLAGS = -D__GAS__
AM_LDFLAGS = -T mbr.ld 

LINK=@LINKER@ $(AM_LDFLAGS) -o loader

noinst_PROGRAMS = loader


loader_SOURCES = boot.S


EXTRA_DIST = mbr.ld


