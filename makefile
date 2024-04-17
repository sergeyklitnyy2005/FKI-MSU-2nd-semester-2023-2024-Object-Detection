CC = gcc
CFLAGS = -Wall -Wextra -std=c11
LIBS = -lm

all: coloring image run

coloring: coloring.c lodepng.c lodepng.h
	$(CC) $(CFLAGS) -o coloring coloring.c lodepng.c $(LIBS)

image: image.c lodepng.c lodepng.h
	$(CC) $(CFLAGS) -o image image.c lodepng.c $(LIBS)

run: image coloring
	./image
	./coloring

clean:
	rm -f coloring image
