#ifndef TEXT_H
#define TEXT_H

#include <SDL2/SDL.h>

struct Text;
typedef struct Text Text;

Text* Text_Init(
    SDL_Renderer* renderer, const char* text, const char* fontfile, int x, int y, int width,
    int height
);
void Text_Render(Text* text);

#endif
