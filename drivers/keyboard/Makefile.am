AM_CPPFLAGS = -m32 -Wall -fmessage-length=0
AM_CCASFLAGS = -D__GAS__

# sneaky hack
CFLAGS= -g -O3

lib_LIBRARIES = libkeyboard.a

libkeyboard_a_SOURCES = i8042.c stdin.c stdin.h codepage.h

EXTRA_DIST = 