CC = gcc
CFLAGS = -Wall -Wextra -std=c11
LIBS = -lm

all: image run

image: image.c
	$(CC) $(CFLAGS) -o image image.c $(LIBS)

run: image
	./image
	
clean:
	rm -f image
