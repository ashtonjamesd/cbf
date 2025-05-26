CC = gcc
EXEC = build/cbf
SRCS    = $(wildcard src/*.c)
CFLAGS = -Wextra -Wall

all:
	$(CC) $(CFLAGS) $(SRCS) -o $(EXEC)