#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#define CELL_SIZE 8

int r = -1, g = -1, b = -1;

typedef struct {
    SDL_Texture* texture;
    int width;
    int height;
} ImageInfo;

typedef struct {
    char* name;
    int x;
    int y;
    ImageInfo image;
} SpriteData;

ImageInfo loadImage(const char* filename, SDL_Renderer* renderer) {
    ImageInfo imageInfo;
    SDL_Surface* surface = IMG_Load(filename);

    if (!surface) {
        fprintf(stderr, "Error loading image %s: %s\n", filename, IMG_GetError());
        exit(EXIT_FAILURE);
    }

    // Replace color with transparent alpha
    if (r != -1 || g != -1 || b != -1) {
        SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, r, g, b));
    }

    imageInfo.texture = SDL_CreateTextureFromSurface(renderer, surface);
    imageInfo.width = surface->w;
    imageInfo.height = surface->h;

    SDL_FreeSurface(surface);

    return imageInfo;
}

// Function to get the file name without the extension
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


unsigned int adjustToNextPowerOfTwo(unsigned int number) {
    if (number && !(number & (number - 1))) {
        // If the number is already a power of 2, return the same number
        return number;
    }

    unsigned int power = 1;
    while (power < number) {
        power <<= 1;
    }

    return power;
}


int compare(const void* a, const void* b) {
    const ImageInfo* imageA = &((const SpriteData*)a)->image;
    const ImageInfo* imageB = &((const SpriteData*)b)->image;

    // Sort from largest to smallest by area (width x height)
    int areaA = imageA->width * imageA->height;
    int areaB = imageB->width * imageB->height;

    return areaB - areaA;
}

int compare_by_name(const void* a, const void* b) {
    const SpriteData* sprA = (const SpriteData*)a;
    const SpriteData* sprB = (const SpriteData*)b;
    return strcmp(sprA->name, sprB->name);
}

void parseHexColor(const char *hexColor, int *r, int *g, int *b) {
    if (strlen(hexColor) == 3 && sscanf(hexColor, "%1x%1x%1x", r, g, b) == 3) return;
    else if (strlen(hexColor) == 6 && sscanf(hexColor, "%1x%1x%1x", r, g, b) == 3) return;
    fprintf(stderr, "Invalid hex color format. Please provide a valid 3-digit or 6-digit hex color value.\n");
    exit(EXIT_FAILURE);
}

int main(int argc, char* argv[]) {
    int totalSprites = 0;

    const char* outputFileName = NULL;
    int sort = 0;
    int sortSpritesNames = 0;
    int error = 0;

    int opt;
    while ((opt = getopt(argc, argv, "o:c:s:n")) != -1) {
        switch (opt) {
            case 'o':
                outputFileName = optarg;
                break;
            case 'c':
                parseHexColor(optarg, &r, &g, &b);
                break;
            case 's':
                sort = 1;
                break;
            case 'n':
                sortSpritesNames = 1;
                break;
            default:
                error = 1;
                return EXIT_FAILURE;
        }
    }

    if (error || outputFileName == NULL || optind == argc) {
        fprintf(stderr, "Usage: %s -o <file> [-c <R> <G> <B>] [-s][-n] image1.png image2.png ... imageN.png\n", argv[0]);
        fprintf(stderr, " options:\n");
        fprintf(stderr, "    -c  set color to transparent alpha in resulting images\n");
        fprintf(stderr, "    -o  output file\n");
        fprintf(stderr, "    -s  sort images by size (in atlas)\n");
        fprintf(stderr, "    -n  sort images ids by name (in .spr file)\n");
        return EXIT_FAILURE;
    }

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "Error initializing SDL: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    SDL_Window* window = SDL_CreateWindow("Image Atlas", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, 0);
    if (!window) {
        fprintf(stderr, "Error creating window: %s\n", SDL_GetError());
        SDL_Quit();
        return EXIT_FAILURE;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        fprintf(stderr, "Error creating renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return EXIT_FAILURE;
    }

    totalSprites = argc - optind;

    SpriteData * sprites = (SpriteData*)malloc(totalSprites * sizeof(SpriteData));

    // Load images
    for (int i = 0; i < totalSprites; ++i) {
        sprites[i].image = loadImage(argv[optind+i], renderer);
        sprites[i].name = strdup(argv[optind+i]);
    }

    // Sort images from largest to smallest by area
    if ( sort ) qsort(sprites, totalSprites, sizeof(SpriteData), compare);

    // Simulate filling to get the best atlas size

    // Calculate atlas dimensions
    int maxWidth = 0;
    int maxHeight = 0;
    for (int i = 0; i < totalSprites; ++i) {
        if (sprites[i].image.width > maxWidth) {
            maxWidth = sprites[i].image.width;
        }
        if (sprites[i].image.height > maxHeight) {
            maxHeight = sprites[i].image.height;
        }
    }

    int size = maxHeight > maxWidth ? maxHeight : maxWidth;
    int usedWidth = 0, usedHeight = 0;  // Actual size occupied by the images

    int fail = 1;

    while( fail ) {
        fail = 0;

        usedWidth = 0, usedHeight = 0;  // Actual size occupied by the images

//        size = adjustToNextPowerOfTwo(size);

        int atlasWidthCells = size / CELL_SIZE;
        int atlasHeightCells = size / CELL_SIZE;

        int* cellMap = (int*)calloc(atlasWidthCells * atlasHeightCells, sizeof(int));

        // Place images on the atlas
        for (int i = 0; i < totalSprites; ++i) {
            int imageWidthCells = (sprites[i].image.width + CELL_SIZE - 1) / CELL_SIZE;
            int imageHeightCells = (sprites[i].image.height + CELL_SIZE - 1) / CELL_SIZE;

            int startX = -1, startY = -1;

            // Find an empty space to place the image on the atlas
            for (int y = 0; y <= atlasHeightCells - imageHeightCells; ++y) {
                for (int x = 0; x <= atlasWidthCells - imageWidthCells; ++x) {
                    int spaceAvailable = 1;
                    // Check if there is enough empty space for the image at this position
                    for (int dy = 0; dy < imageHeightCells; ++dy) {
                        for (int dx = 0; dx < imageWidthCells; ++dx) {
                            if (cellMap[(y + dy) * atlasWidthCells + (x + dx)] == 1) {
                                spaceAvailable = 0; // Not enough space
                                break;
                            }
                        }
                        if (!spaceAvailable) {
                            break;
                        }
                    }
                    if (spaceAvailable) {
                        startX = x;
                        startY = y;
                        break;
                    }
                }
                if (startX != -1) {
                    break;
                }
            }

            if (startX != -1) {
                sprites[i].x = startX;
                sprites[i].y = startY;
                // Mark occupied cells on the map
                for (int dy = 0; dy < imageHeightCells; ++dy) {
                    for (int dx = 0; dx < imageWidthCells; ++dx) {
                        cellMap[(startY + dy) * atlasWidthCells + (startX + dx)] = 1;
                    }
                }
                // Update actual size occupied by the images
                int imageRight = startX * CELL_SIZE + sprites[i].image.width;
                int imageBottom = startY * CELL_SIZE + sprites[i].image.height;
                if (imageRight > usedWidth) {
                    usedWidth = imageRight;
                }
                if (imageBottom > usedHeight) {
                    usedHeight = imageBottom;
                }
            } else {
                fail = 1;
            }
        }

        free(cellMap);

        if ( fail ) size += CELL_SIZE; //size++;

    }

    if ( sortSpritesNames ) qsort(sprites, totalSprites, sizeof(SpriteData), compare_by_name);

    int atlasWidth = usedWidth;
    int atlasHeight = usedHeight;

    // Create renderer and atlasTexture
    SDL_Texture* atlasTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, atlasWidth, atlasHeight);
    SDL_SetRenderTarget(renderer, atlasTexture);
    
    // Clear the atlas
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);

    char * fileNameWithoutExtension = getFileNameWithoutExtension( outputFileName );
    char * sprOutFile = malloc( strlen( fileNameWithoutExtension ) + 4 );
    sprintf(sprOutFile, "%s.spr", fileNameWithoutExtension);
    FILE * sprFP = fopen( sprOutFile, "w" );
    free(fileNameWithoutExtension);
    free(sprOutFile);

    fprintf(sprFP,"sprites=%d\n", totalSprites);

    // Place images on the atlas
    for (int i = 0; i < totalSprites; ++i) {
        // Place the image in the found space
        int destX = sprites[i].x * CELL_SIZE;
        int destY = sprites[i].y * CELL_SIZE;

        SDL_Rect srcRect = {0, 0, sprites[i].image.width, sprites[i].image.height};
        SDL_Rect destRect = {destX, destY, sprites[i].image.width, sprites[i].image.height};
        SDL_RenderCopy(renderer, sprites[i].image.texture, &srcRect, &destRect);

        char* fileNameWithoutExtension = getFileNameWithoutExtension(sprites[i].name);
        fprintf(sprFP,"\n[%d]\nname=%s\nclip=%d,%d,%d,%d\n", i + 1, fileNameWithoutExtension, destX, destY, sprites[i].image.width, sprites[i].image.height);
        free(fileNameWithoutExtension);
        fprintf(sprFP,"center=%f,%f\n", sprites[i].image.width / 2.0, sprites[i].image.height / 2.0);
        fprintf(sprFP,"cpoints=0\n");
        fprintf(sprFP,";cpoints[0]=0,0\n");
    }

    fclose(sprFP);

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
    // Create final surface and save the atlas
    SDL_Surface* finalSurface = SDL_CreateRGBSurface(0, usedWidth, usedHeight, 32, rmask, gmask, bmask, amask);
    SDL_RenderReadPixels(renderer, NULL, SDL_PIXELFORMAT_ARGB8888, finalSurface->pixels, finalSurface->pitch);

    IMG_SavePNG(finalSurface, outputFileName);

    // Free memory and resources
    for (int i = 0; i < totalSprites; ++i) {
        SDL_DestroyTexture(sprites[i].image.texture);
        free(sprites[i].name);
    }

    free(sprites);

    SDL_DestroyTexture(atlasTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return EXIT_SUCCESS;
}
