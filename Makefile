.DEFAULT_GOAL=main

CC=gcc
PROG=gol
CFLAGS=-Wall -Wextra -Wno-format -Werror
CFLAGS+=-std=c2x
LIBS=
INCLUDES=
SRCS=\
	gol.c

main:
	$(CC) $(CFLAGS) $(SRCS) -o $(PROG)
