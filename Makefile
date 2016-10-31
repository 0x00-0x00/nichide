CC=gcc
CFLAGS=-std=gnu99 -Wall
PROG = nichide

nichide: main.c
	$(CC) -o $(PROG) main.c $(CFLAGS)

install:
	unlink /usr/bin/nichide 2>&1
	install -m 0755 nichide /bin
	ln -s /bin/nichide /usr/bin/nichide

.PHONY: install
