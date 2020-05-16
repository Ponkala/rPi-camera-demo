

.POSIX:

BIN=main
LDFLAGS= `pkg-config --cflags gtk+-2.0`
LDLIBS= `pkg-config --libs gtk+-2.0`
$(CFLAGS)= -g -std=c99

CC = gcc
LD = gcc
all: main

$(BIN).o: $(BIN).c

.c.o:
	 $(CC) -o $@ -c $(CFLAGS) $(LDFLAGS) $< -lm

$(BIN): $(BIN).o
	$(LD) -o $@ $< $(LDLIBS) -lm


