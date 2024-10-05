#include "text.h"

#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_ttf.h>
#include <stdlib.h>

typedef struct Text {
    const char* text;
    SDL_Rect rect;
    TTF_Font* font;
    SDL_Surface* surface;
    SDL_Texture* texture;
    SDL_Renderer* renderer;
} Text;

Text* Text_Init(
    SDL_Renderer* renderer, const char* text, const char* fontfile, int x, int y, int width,
    int height
) {
    Text* new = malloc(sizeof(Text));
    new->text = text;
    new->font = TTF_OpenFont(fontfile, 128);
    new->renderer = renderer;

    SDL_Color color = {255, 255, 255, 255};
    new->surface = TTF_RenderText_Solid(new->font, new->text, color);

    new->rect.x = x;
    new->rect.y = y;
    new->rect.w = width;
    new->rect.h = height;

    new->texture = SDL_CreateTextureFromSurface(renderer, new->surface);
    SDL_FreeSurface(new->surface);

    return new;
}

void Text_Render(Text* text) { SDL_RenderCopy(text->renderer, text->texture, NULL, &text->rect); }
