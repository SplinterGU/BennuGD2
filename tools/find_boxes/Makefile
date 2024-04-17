CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -pedantic -ggdb
LDFLAGS = -lSDL2 -lSDL2_image -lpng -lz

all: find_boxes

find_boxes: find_boxes.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

find_boxes.o: find_boxes.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f find_boxes *.o
