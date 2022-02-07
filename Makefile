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
LEX              =   flex
YACC             =   bison -y
YFLAGS           =   -d

all: pixie

pixie: pixie.o parser.o lexer.o gui.o panic.o args.o pipes.o exec.o

clean: 
	rm -f pixie  *.o *~ y.tab.h parser.tab.*
