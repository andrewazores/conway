all:
	mkdir -p gen
	gcc -std=c99 -Wall conway.c -o gen/conway.o -lGL -lGLU -lglut

clean:
	rm -rf gen
