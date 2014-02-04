CC=gcc
CFLAGS=-std=gnu99 -Wall -Wextra -Wdouble-promotion -Wformat -Wconversion -Wpedantic
LIBS=-lGL -lGLU -lglut
all:
	mkdir -p gen
	$(CC) $(CFLAGS) conway.c -o gen/conway.o $(LIBS)

clean:
	rm -rf gen
