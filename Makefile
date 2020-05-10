CC = gcc
CFLAGS = -Wall -Wextra
PROGS = client server

.PHONY: clean all build

all: build

build: client server

client: client.o

server: server.o

client.o: client.c

server.o: server.c

clean:
	@rm -v *.o $(PROGS)
