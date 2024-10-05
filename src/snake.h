#ifndef SNAKE_H
#define SNAKE_H

#include <SDL2/SDL.h>
#include <stdbool.h>

enum Direction { UP, DOWN, LEFT, RIGHT };

struct Node {
    SDL_Rect rect;
    enum Direction direction;
    struct Node* next;
};

struct Node* snake_get_node();

void snake_init(SDL_Renderer* renderer, int x, int y, int size);
void snake_movement(const Uint8* keyboard);
void snake_grow_snake();
void snake_reset();
bool snake_intersection();
void snake_render(SDL_Renderer* renderer);
void snake_quit();

#endif
