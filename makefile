CC = gcc
EXEC = build/cbf
OBJS = src/main.c

all:
	$(CC) $(OBJS) -o $(EXEC)