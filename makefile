CC = gcc
EXEC = build/cbf
OBJS = src/main.c src/tokenize.c

all:
	$(CC) $(OBJS) -o $(EXEC)