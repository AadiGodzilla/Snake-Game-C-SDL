#include "snake.h"
#include <SDL2/SDL_image.h>
#include <stdlib.h>

struct Snake {
    int x, y, size;
    int defaultsize;
    struct Node* head;
    enum Direction dir;
    SDL_Texture* images[14];
};

struct Snake snake;

void insertFront(struct Node** head, SDL_Rect rect) {
    struct Node* new = malloc(sizeof(struct Node));
    new->rect = rect;
    new->direction = (*head)->direction;
    new->next = (*head);
    (*head) = new;
}

void deleteBack(struct Node** head) {
    struct Node* current = (*head);

    if (current->next == NULL) {
        return;
    }

    while (current->next->next != NULL) {
        current = current->next;
    }

    free(current->next);
    current->next = NULL;
}

struct Node* snake_get_node() { return snake.head; }

int snake_get_size() {
    struct Node* head = snake.head;
    int length = 0;
    while (head != NULL) {
        if (head != snake.head->next) {
            length++;
        }
        head = head->next;
    }
    return length;
}

struct Node* snake_get_tail() {
    struct Node* tail = snake.head;
    while (tail->next != NULL) {
        tail = tail->next;
    }
    return tail;
}

void snake_init(SDL_Renderer* renderer, int x, int y, int size) {
    snake.x = x;
    snake.y = y;
    snake.size = size;
    snake.defaultsize = 4;
    snake.dir = RIGHT;

    SDL_Rect head_rect = {snake.x, snake.y, snake.size, snake.size};
    snake.head = malloc(sizeof(struct Node));
    snake.head->rect = head_rect;
    snake.head->direction = snake.dir;
    snake.head->next = NULL;

    struct Node* head = snake.head;
    while (snake_get_size() != snake.defaultsize) {
        SDL_Rect rect = head->rect;
        rect.x = rect.x + snake.size;
        insertFront(&snake.head, rect);
    }

    snake.images[0] = SDL_CreateTextureFromSurface(renderer, IMG_Load("res/body_tl.png"));
    snake.images[1] = SDL_CreateTextureFromSurface(renderer, IMG_Load("res/body_tr.png"));
    snake.images[2] = SDL_CreateTextureFromSurface(renderer, IMG_Load("res/body_bl.png"));
    snake.images[3] = SDL_CreateTextureFromSurface(renderer, IMG_Load("res/body_br.png"));

    snake.images[4] = SDL_CreateTextureFromSurface(renderer, IMG_Load("res/body_horizontal.png"));
    snake.images[5] = SDL_CreateTextureFromSurface(renderer, IMG_Load("res/body_vertical.png"));

    snake.images[6] = SDL_CreateTextureFromSurface(renderer, IMG_Load("res/head_up.png"));
    snake.images[7] = SDL_CreateTextureFromSurface(renderer, IMG_Load("res/head_down.png"));
    snake.images[8] = SDL_CreateTextureFromSurface(renderer, IMG_Load("res/head_left.png"));
    snake.images[9] = SDL_CreateTextureFromSurface(renderer, IMG_Load("res/head_right.png"));

    snake.images[10] = SDL_CreateTextureFromSurface(renderer, IMG_Load("res/tail_up.png"));
    snake.images[11] = SDL_CreateTextureFromSurface(renderer, IMG_Load("res/tail_down.png"));
    snake.images[12] = SDL_CreateTextureFromSurface(renderer, IMG_Load("res/tail_left.png"));
    snake.images[13] = SDL_CreateTextureFromSurface(renderer, IMG_Load("res/tail_right.png"));
}

void snake_grow_snake() { snake.defaultsize++; }

void snake_reset() {
    snake.defaultsize = 4;
    snake.dir = RIGHT;
    snake.head->rect.x = snake.x;
    snake.head->rect.y = snake.y;
    while (snake_get_size() != 1) {
        deleteBack(&snake.head);
    }

    struct Node* head = snake.head;
    while (snake_get_size() < snake.defaultsize) {
        head->rect.x += snake.size;
        insertFront(&snake.head, head->rect);
    }
}

void snake_movement(const Uint8* keyboard) {
    if (keyboard[SDL_SCANCODE_UP] && snake.dir != DOWN) {
        snake.dir = UP;
    } else if (keyboard[SDL_SCANCODE_DOWN] && snake.dir != UP) {
        snake.dir = DOWN;
    } else if (keyboard[SDL_SCANCODE_LEFT] && snake.dir != RIGHT) {
        snake.dir = LEFT;
    } else if (keyboard[SDL_SCANCODE_RIGHT] && snake.dir != LEFT) {
        snake.dir = RIGHT;
    }

    switch (snake.dir) {
    case UP:
        snake.head->rect.y -= snake.size;
        break;
    case DOWN:
        snake.head->rect.y += snake.size;
        break;
    case LEFT:
        snake.head->rect.x -= snake.size;
        break;
    case RIGHT:
        snake.head->rect.x += snake.size;
        break;
    }

    snake.head->direction = snake.dir;

    insertFront(&snake.head, snake.head->rect);
    if (snake_get_size() > snake.defaultsize) {
        deleteBack(&snake.head);
    }
}

bool snake_intersection() {
    if (snake.head->next != NULL && snake.head->next->next != NULL) {
        struct Node* head = snake.head->next->next;

        while (head != NULL) {
            if (SDL_HasIntersection(&head->rect, &snake_get_node()->rect)) {
                return true;
            }
            head = head->next;
        }
        return false;
    }
    return false;
}

void snake_render(SDL_Renderer* renderer) {
    struct Node* pivot = snake.head;

    while (pivot != NULL && pivot->next != NULL && pivot->next->next != NULL) {
        if (pivot != snake.head && pivot->next->direction == pivot->direction) {
            if ((pivot->direction == UP || pivot->direction == DOWN)) {
                SDL_RenderCopy(renderer, snake.images[5], NULL, &pivot->next->rect);
            } else if ((pivot->direction == LEFT || pivot->direction == RIGHT)) {
                SDL_RenderCopy(renderer, snake.images[4], NULL, &pivot->next->rect);
            }
        }
        if ((pivot->direction == DOWN && pivot->next->direction == RIGHT) ||
            (pivot->direction == LEFT && pivot->next->direction == UP)) {
            SDL_RenderCopy(renderer, snake.images[2], NULL, &pivot->next->rect);
        } else if ((pivot->direction == LEFT && pivot->next->direction == DOWN) ||
                   (pivot->direction == UP && pivot->next->direction == RIGHT)) {
            SDL_RenderCopy(renderer, snake.images[0], NULL, &pivot->next->rect);
        } else if ((pivot->direction == UP && pivot->next->direction == LEFT) ||
                   (pivot->direction == RIGHT && pivot->next->direction == DOWN)) {
            SDL_RenderCopy(renderer, snake.images[1], NULL, &pivot->next->rect);
        } else if ((pivot->direction == RIGHT && pivot->next->direction == UP) ||
                   (pivot->direction == DOWN && pivot->next->direction == LEFT)) {
            SDL_RenderCopy(renderer, snake.images[3], NULL, &pivot->next->rect);
        }

        pivot = pivot->next;
    }

    struct Node* tail = snake_get_tail();

    if (pivot->direction == UP) {
        SDL_RenderCopy(renderer, snake.images[11], NULL, &tail->rect);
    } else if (pivot->direction == DOWN) {
        SDL_RenderCopy(renderer, snake.images[10], NULL, &tail->rect);
    } else if (pivot->direction == LEFT) {
        SDL_RenderCopy(renderer, snake.images[13], NULL, &tail->rect);
    } else if (pivot->direction == RIGHT) {
        SDL_RenderCopy(renderer, snake.images[12], NULL, &tail->rect);
    }

    if (snake.dir == UP) {
        SDL_RenderCopy(renderer, snake.images[6], NULL, &snake.head->rect);
    } else if (snake.dir == DOWN) {
        SDL_RenderCopy(renderer, snake.images[7], NULL, &snake.head->rect);
    } else if (snake.dir == LEFT) {
        SDL_RenderCopy(renderer, snake.images[8], NULL, &snake.head->rect);
    } else if (snake.dir == RIGHT) {
        SDL_RenderCopy(renderer, snake.images[9], NULL, &snake.head->rect);
    }
}
