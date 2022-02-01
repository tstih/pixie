#
# pixie/Makefile
#
# Makefile for the pixie.
#
# Copyright 2022 Tomaz Stih
#
# 2022-02-01	tstih
#
CC               =   gcc
CFLAGS           =   -Wall -g
LDLIBS           =   -lSDL2

all: pixie

pixie: pixie.o

clean: 
	rm -f pixie  *.o *~
