#ifndef SCORE_H
#define SCORE_H

#include <SDL2/SDL.h>

void score_init(const char* fontfile);
void score_add();
void score_reset();
void score_render(SDL_Renderer* renderer);

#endif
