#ifndef APPLE_H
#define APPLE_H

#include <SDL2/SDL.h>

SDL_Rect *apple_get_rect();

void apple_init(int x, int y, int size, int cellrows, int cellcols);
void apple_randomize();
void apple_render(SDL_Renderer *renderer);

#endif
