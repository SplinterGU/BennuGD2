#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <png.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#define DEFAULT_WEIGHT_THRESHOLD    0

#define WEIGHT_MODE_ALPHA           0
#define WEIGHT_MODE_COLOR           1

// Structure to represent a box
typedef struct {
    int x;
    int y;
    int w;
    int h;
} Box;

int weight_threshold = DEFAULT_WEIGHT_THRESHOLD;
int weight_mode = WEIGHT_MODE_ALPHA;
unsigned int color_r = 0, color_g = 0, color_b = 0;
int max_box = 0;
int alpha_threshold = 0;

char * visited = NULL;
char * weights = NULL;

int is_valid_pixel(SDL_Surface *image, int x, int y ) {
    Uint8 r, g, b, a;
    Uint32 pixel = *((Uint32 *)image->pixels + y * image->pitch / sizeof(Uint32) + x);
    SDL_GetRGBA(pixel, image->format, &r, &g, &b, &a);
    if (weight_mode == WEIGHT_MODE_ALPHA) {
        if (a > alpha_threshold) return 1;
    } else {
        if (pixel != SDL_MapRGB(image->format, color_r, color_g, color_b)) return 1;
    }
    return 0;
}

// Function to calculate the weight of each cell based on its surrounding 8 directions and the specified mode
void calculateWeights(SDL_Surface *image) {
    int rows = image->h;
    int cols = image->w;

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            int weight = is_valid_pixel(image, j, i);
            if ( weight ) {
                for (int di = -1; di <= 1; ++di) {
                    for (int dj = -1; dj <= 1; ++dj) {
                        if (di == 0 && dj == 0) continue; // Skip the current cell
                        int ni = i + di;
                        int nj = j + dj;
                        if (ni >= 0 && ni < rows && nj >= 0 && nj < cols) {
                            weight += is_valid_pixel(image, nj, ni);
                        }
                    }
                }
            }
            // Assign the weight to the current cell
            weights[i * cols + j] = weight;
        }
    }
}

#if 0
// Function to find boxes formed by cells with weight greater than "weight_threshold"
int findBoxes(SDL_Surface *image, int r, int c, Box* box) {
    int rows = image->h;
    int cols = image->w;

    if (r < 0 || r >= rows || c < 0 || c >= cols || visited[r * cols + c] || weights[r * cols + c] <= weight_threshold) return 0;
    visited[r * cols + c] = 1; // Mark the cell as visited

    // Update the dimensions of the box
    if (r < box->y) {
        box->h += box->y - r;
        box->y = r;
    }
    if (c < box->x) {
        box->w += box->x - c;
        box->x = c;
    }
    if (r - box->y + 1 > box->h) box->h = r - box->y + 1;
    if (c - box->x + 1 > box->w) box->w = c - box->x + 1;

    // Recursively explore adjacent cells
    findBoxes(image, r + 1, c, box);
    findBoxes(image, r - 1, c, box);
    findBoxes(image, r, c + 1, box);
    findBoxes(image, r, c - 1, box);

    return 1;
}
#else

// Function to find boxes formed by cells with weight greater than "weight_threshold"
int findBoxes(SDL_Surface *image, int r, int c, Box* box) {
    int rows = image->h;
    int cols = image->w;

    // Array to store the coordinates of cells to be visited
    int *stack = (int *)calloc(rows * cols * 2 * 4, sizeof(int));
    if (stack == NULL) {
        fprintf(stderr, "Memory allocation failed for the stack.\n");
        exit(EXIT_FAILURE);
    }
    int top = -1; // Index of the top element in the stack

    // Push the initial coordinates onto the stack
    stack[++top] = r;
    stack[++top] = c;

    while (top >= 0) {
        // Pop the top element from the stack
        int cur_c = stack[top--];
        int cur_r = stack[top--];

        // Check if the current cell is within bounds and has not been visited
        if (cur_r < 0 || cur_r >= rows || cur_c < 0 || cur_c >= cols || visited[cur_r * cols + cur_c] || weights[cur_r * cols + cur_c] <= weight_threshold ) continue;

        // Mark the current cell as visited
        visited[cur_r * cols + cur_c] = 1;

        // Update the dimensions of the box
        if (cur_r < box->y) {
            box->h += box->y - cur_r;
            box->y = cur_r;
        }
        if (cur_c < box->x) {
            box->w += box->x - cur_c;
            box->x = cur_c;
        }
        if (cur_r - box->y + 1 > box->h) box->h = cur_r - box->y + 1;
        if (cur_c - box->x + 1 > box->w) box->w = cur_c - box->x + 1;

        // Push the coordinates of the neighboring cells onto the stack
        stack[++top] = cur_r + 1; // Down
        stack[++top] = cur_c;
        stack[++top] = cur_r - 1; // Up
        stack[++top] = cur_c;
        stack[++top] = cur_r;     // Right
        stack[++top] = cur_c + 1;
        stack[++top] = cur_r;     // Left
        stack[++top] = cur_c - 1;
    }

    // Free the memory allocated for the stack
    free(stack);

    return 1;
}
#endif

// Function to find all boxes in the image
void findAllBoxes(SDL_Surface *image) {
    int rows = image->h;
    int cols = image->w;

    // Dynamically allocate the visited array
    visited = (char *)calloc(rows * cols, sizeof(char));
    if (visited == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        return;
    }

    // Array to store the weights of each cell
    weights = (char *)calloc(rows * cols, sizeof(int));
    if (weights == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        free(visited);
        return;
    }

    // Variables to track total bounds
    int min_x = cols;
    int max_x = 0;
    int min_y = rows;
    int max_y = 0;

    // Calculate weights based on alpha threshold
    calculateWeights(image);

    int boxid = 0;

    // Find boxes and display their information
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (weights[i * cols + j] > weight_threshold && !visited[i * cols + j]) {
                Box box = {j, i, 1, 1}; // Initialize the box with the current position (x, y)
                findBoxes(image, i, j, &box);
                if ( !max_box ) printf("%d, %d, %d, %d, %d\n", ++boxid, box.x, box.y, box.w, box.h);
                // Update total bounds
                if (box.x < min_x) min_x = box.x;
                if (box.y < min_y) min_y = box.y;
                if (box.x + box.w - 1 > max_x) max_x = box.x + box.w - 1;
                if (box.y + box.h - 1 > max_y) max_y = box.y + box.h - 1;
            }
        }
    }

    // Calculate and display the total bounding box
    int width = max_x - min_x + 1;
    int height = max_y - min_y + 1;
    if ( max_box ) printf("%d, %d, %d, %d, %d\n", ++boxid, min_x, min_y, width, height);

    // Free dynamically allocated memory
    free(weights);
    free(visited);
}

void parseHexColor(const char *hexColor, unsigned int *r, unsigned int *g, unsigned int *b) {
    if (strlen(hexColor) == 3 && sscanf(hexColor, "%1x%1x%1x", r, g, b) == 3) return;
    else if (strlen(hexColor) == 6 && sscanf(hexColor, "%1x%1x%1x", r, g, b) == 3) return;
    fprintf(stderr, "Invalid hex color format. Please provide a valid 3-digit or 6-digit hex color value.\n");
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s -i <image_file> [-t <threshold>] [-a <alpha_threshold>] [-c <R> <G> <B>] [-m <mode>] \n", argv[0]);
        fprintf(stderr, " options:\n");
        fprintf(stderr, "    -i  input image file\n");
        fprintf(stderr, "    -t  set weight threshold for box detection (default: %d)\n", DEFAULT_WEIGHT_THRESHOLD);
        fprintf(stderr, "    -a  set alpha threshold for pixel processing (default: 0)\n");
        fprintf(stderr, "    -c  set color for transparent alpha in resulting images\n");
        fprintf(stderr, "    -m  set mode: 0 for individual boxes, 1 for total bounding box\n");
        return 1;
    }

    char *image_file = NULL;

    // Parse command line arguments
    int opt;
    while ((opt = getopt(argc, argv, "i:t:a:c:m")) != -1) {
        switch (opt) {
            case 'i':
                // Process input image file
                image_file = optarg;
                break;
            case 't':
                // Set weight threshold for box detection
                weight_threshold = atoi(optarg);
                break;
            case 'a':
                // Set alpha threshold for pixel processing
                alpha_threshold = atoi(optarg);
                weight_mode = WEIGHT_MODE_ALPHA;
                break;
            case 'c':
                // Set color for transparent alpha
                weight_mode = WEIGHT_MODE_COLOR;
                parseHexColor(optarg, &color_r, &color_g, &color_b);
                break;
            case 'm':
                max_box = 1;
                break;
            default:
                // Print usage message for invalid options
                fprintf(stderr, "Invalid option\n");
                fprintf(stderr, "Usage: %s -i <image_file> [-t <threshold>] [-a <alpha_threshold>] [-c <R> <G> <B>] [-m <mode>] \n", argv[0]);
                fprintf(stderr, " options:\n");
                fprintf(stderr, "    -i  input image file\n");
                fprintf(stderr, "    -t  set weight threshold for box detection (default: %d)\n", DEFAULT_WEIGHT_THRESHOLD);
                fprintf(stderr, "    -a  set alpha threshold for pixel processing (default: 0)\n");
                fprintf(stderr, "    -c  set color for transparent alpha in resulting images\n");
                fprintf(stderr, "    -m  get total bounding box\n");
                return EXIT_FAILURE;
        }
    }

    // Check if input image file is provided
    if (image_file == NULL) {
        fprintf(stderr, "Input image file is missing.\n");
        fprintf(stderr, "Usage: %s -i <image_file> [-t <threshold>] [-a <alpha_threshold>] [-c <R> <G> <B>] [-m <mode>] \n", argv[0]);
        fprintf(stderr, " options:\n");
        fprintf(stderr, "    -i  input image file\n");
        fprintf(stderr, "    -t  set weight threshold for box detection (default: %d)\n", DEFAULT_WEIGHT_THRESHOLD);
        fprintf(stderr, "    -a  set alpha threshold for pixel processing (default: 0)\n");
        fprintf(stderr, "    -c  set color for transparent alpha in resulting images\n");
        fprintf(stderr, "    -m  get total bounding box\n");
        return EXIT_FAILURE;
    }

    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "SDL_Init Error: %s\n", SDL_GetError());
        return 1;
    }

    // Initialize SDL_image
    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        fprintf(stderr, "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
        SDL_Quit();
        return 1;
    }

    // Load the image
    SDL_Surface *image = IMG_Load(image_file);
    if (!image) {
        fprintf(stderr, "Unable to load image %s! SDL_image Error: %s\n", image_file, IMG_GetError());
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    // Find boxes in the image
    findAllBoxes(image);

    // Free the image surface
    SDL_FreeSurface(image);

    // Quit SDL_image
    IMG_Quit();

    // Quit SDL
    SDL_Quit();

    return 0;
}
