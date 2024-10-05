#include "score.h"

#include <SDL2/SDL_ttf.h>
#include <string.h>

struct Score {
    SDL_Rect rect;
    TTF_Font* font;
    int scorecount;
    SDL_Surface* surface;
    SDL_Texture* texture;
};

struct Score score;

void score_init(const char* fontfile) {
    score.scorecount = 0;

    score.font = TTF_OpenFont(fontfile, 128);

    score.rect.x = 20;
    score.rect.y = 20;
    score.rect.w = 110;
    score.rect.h = 30;
}

void score_add() { score.scorecount++; }

void score_reset() { score.scorecount = 0; }

void score_render(SDL_Renderer* renderer) {
    char score_prefix[20] = "Score: ";
    char score_str[10];

    sprintf(score_str, "%d", score.scorecount);
    strcat(score_prefix, score_str);

    SDL_Color color = {255, 255, 255, 255};
    score.surface = TTF_RenderText_Solid(score.font, score_prefix, color);
    score.texture = SDL_CreateTextureFromSurface(renderer, score.surface);

    SDL_FreeSurface(score.surface);
    SDL_RenderCopy(renderer, score.texture, NULL, &score.rect);
}
