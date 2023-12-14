CC = gcc
CFLAGS = -Wall -Wextra
LIBS = -lSDL2 -lSDL2_image

all: split_image

split_image: split_image.c
	$(CC) $(CFLAGS) split_image.c -o split_image $(LIBS)

clean:
	rm -f split_image
