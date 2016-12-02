CC=gcc
CFLAGS=-std=gnu99 -Wall
PROG = nichide

nichide: main.c
	$(CC) -o $(PROG) main.c $(CFLAGS)

install:
	unlink /usr/local/bin/nichide 2>&1
	install -m 0755 nichide /usr/local/bin
	ln -s /usr/local/bin/nichide /usr/bin/nichide

.PHONY: install
