CC = gcc
CFLAGS = -Wall -Wextra
LIBS = -lSDL2 -lSDL2_image

all: make_atlas

make_atlas: make_atlas.c
	$(CC) $(CFLAGS) make_atlas.c -o make_atlas $(LIBS)

clean:
	rm -f make_atlas
