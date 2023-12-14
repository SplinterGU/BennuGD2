#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

// Function to get the filename without extension
char* getFileNameWithoutExtension(const char* filename) {
    char* dot = strrchr(filename, '.');
    if (!dot || dot == filename) {
        return strdup(filename);
    } else {
        size_t len = dot - filename;
        char* name = (char*)malloc(len + 1);
        memcpy(name, filename, len);
        name[len] = '\0';
        return name;
    }
}

int main(int argc, char* argv[]) {
    if (argc < 4 || argc > 7) {
        fprintf(stderr, "Usage: %s <image.png> <divisions_width> <divisions_height> [optional: <R> <G> <B>]\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char* filename = argv[1];
    int divisionsX = atoi(argv[2]);
    int divisionsY = atoi(argv[3]);

    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("Image Division", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_Surface* surface = IMG_Load(filename);

    if (!surface) {
        fprintf(stderr, "Error loading image %s: %s\n", filename, IMG_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return EXIT_FAILURE;
    }

    if (argc == 7) {
        Uint8 r = atoi(argv[4]);
        Uint8 g = atoi(argv[5]);
        Uint8 b = atoi(argv[6]);

        SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, r, g, b));
    }

    int width = surface->w;
    int height = surface->h;

    int segmentWidth = width / divisionsX;
    int segmentHeight = height / divisionsY;

    char *name = getFileNameWithoutExtension(filename);

    Uint32 rmask, gmask, bmask, amask;
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    amask = 0x000000ff;
    rmask = 0x0000ff00;
    gmask = 0x00ff0000;
    bmask = 0xff000000;
#else
    amask = 0xff000000;
    rmask = 0x00ff0000;
    gmask = 0x0000ff00;
    bmask = 0x000000ff;
#endif

    for (int y = 0; y < divisionsY; ++y) {
        for (int x = 0; x < divisionsX; ++x) {
            SDL_Rect segmentRect = { x * segmentWidth, y * segmentHeight, segmentWidth, segmentHeight };
            SDL_Surface* segmentSurface = SDL_CreateRGBSurface(0, segmentWidth, segmentHeight, 32, rmask, gmask, bmask, amask);
            SDL_BlitSurface(surface, &segmentRect, segmentSurface, NULL);

            char outputFilename[100];
            snprintf(outputFilename, sizeof(outputFilename), "%s-%d.png", name, y * divisionsX + x + 1);

            IMG_SavePNG(segmentSurface, outputFilename);

            SDL_FreeSurface(segmentSurface);
        }
    }

    free(name);

    SDL_FreeSurface(surface);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return EXIT_SUCCESS;
}
