CC=gcc
CFLAGS=-I.
DEPS = buffer.h
OBJ = main.o buffer.o 


all: main

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

main: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

run:
	./main
