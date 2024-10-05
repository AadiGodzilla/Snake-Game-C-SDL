#include "apple.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdlib.h>
#include <time.h>

struct Apple {
    int x, y, size;
    int cellrows, cellcols;
    SDL_Rect rect;
    SDL_Surface* surface;
    SDL_Texture* texture;
};

struct Apple apple;

SDL_Rect* apple_get_rect() { return &apple.rect; }

void apple_init(int x, int y, int size, int cellrows, int cellcols) {
    apple.x = x;
    apple.y = y;
    apple.size = size;
    apple.cellrows = cellrows;
    apple.cellcols = cellcols;

    apple.rect.x = apple.x;
    apple.rect.y = apple.y;
    apple.rect.w = apple.size;
    apple.rect.h = apple.size;

    apple.surface = IMG_Load("res/apple.png");
}

void apple_randomize() {
    srand(time(0));

    int randomX = ((rand() % apple.cellcols - 1) + 1) * apple.size;
    int randomY = ((rand() % apple.cellrows - 1) + 1) * apple.size;

    while (randomX == apple.rect.x || randomY == apple.rect.y || randomX == 0 || randomY == 0 ||
           randomX == apple.cellcols * apple.size || randomY == apple.cellrows * apple.size) {
        randomX = ((rand() % apple.cellcols - 1) + 1) * apple.size;
        randomY = ((rand() % apple.cellrows - 1) + 1) * apple.size;
    }

    apple.rect.x = randomX;
    apple.rect.y = randomY;
}

void apple_render(SDL_Renderer* renderer) {
    apple.texture = SDL_CreateTextureFromSurface(renderer, apple.surface);

    SDL_RenderCopy(renderer, apple.texture, NULL, &apple.rect);
}
