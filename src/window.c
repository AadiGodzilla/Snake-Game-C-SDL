#include "window.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>
#include <stdlib.h>

#include "apple.h"
#include "score.h"
#include "snake.h"
#include "text.h"

#define CELLSIZE 20
#define CELLCOLS 40
#define CELLROWS 30
#define SCREENWIDTH CELLCOLS* CELLSIZE
#define SCREENHEIGHT CELLROWS* CELLSIZE

struct Window {
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Event event;
    bool running, pause, gameover;
    const Uint8* keyboard;
    SDL_Rect boundaries[4];
    Text *pause_text, *gameover_text;
};

struct Window window;

void window_init(const char* title) {
    SDL_Init(SDL_INIT_EVERYTHING);
    TTF_Init();
    IMG_Init(IMG_INIT_PNG);
    window.window = SDL_CreateWindow(
        title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREENWIDTH, SCREENHEIGHT, 0
    );
    window.renderer = SDL_CreateRenderer(window.window, -1, SDL_RENDERER_ACCELERATED);
    window.running = true;

    snake_init(window.renderer, 3 * CELLSIZE, 7 * CELLSIZE, CELLSIZE);
    apple_init(7 * CELLSIZE, 3 * CELLSIZE, CELLSIZE, CELLROWS - 1, CELLCOLS - 1);
    score_init("res/arial.ttf");

    for (int i = 0; i < 4; i++) {
        if (i < 2) {
            window.boundaries[i].x = 0;
            window.boundaries[i].w = SCREENWIDTH;
            window.boundaries[i].h = CELLSIZE;
            if (i % 2 != 0)
                window.boundaries[i].y = SCREENHEIGHT - CELLSIZE;
            else
                window.boundaries[i].y = 0;
        } else {
            window.boundaries[i].y = 0;
            window.boundaries[i].w = CELLSIZE;
            window.boundaries[i].h = SCREENHEIGHT;
            if (i % 2 != 0)
                window.boundaries[i].x = SCREENWIDTH - CELLSIZE;
            else
                window.boundaries[i].x = 0;
        }
    }

    window.pause_text = Text_Init(
        window.renderer, "PAUSE", "res/arial.ttf", (SCREENWIDTH / 2) - 80, (SCREENHEIGHT / 2) - 30,
        160, 60
    );
    window.gameover_text = Text_Init(
        window.renderer, "GAME OVER", "res/arial.ttf", (SCREENWIDTH / 2) - 150,
        (SCREENHEIGHT / 2) - 30, 300, 60
    );

    SDL_SetRenderDrawBlendMode(window.renderer, SDL_BLENDMODE_BLEND);
}

void window_loop() {
    while (window.running) {
        while (SDL_PollEvent(&window.event)) {
            switch (window.event.type) {
            case SDL_QUIT:
                window.running = false;
                break;
            case SDL_KEYDOWN:
                switch (window.event.key.keysym.sym) {
                case SDLK_ESCAPE:
                    window.pause = !window.pause;
                    break;
                default:
                    window.gameover = false;
                }
                break;
            }

            window.keyboard = SDL_GetKeyboardState(NULL);
        }

        if (!window.pause && !window.gameover) {
            snake_movement(window.keyboard);
            if (snake_intersection()) {
                window.gameover = true;
                snake_reset();
            };
            if (SDL_HasIntersection(&snake_get_node()->rect, apple_get_rect())) {
                snake_grow_snake();
                apple_randomize();
                score_add();
            }
            for (int i = 0; i < sizeof(window.boundaries) / sizeof(SDL_Rect); i++) {
                if (SDL_HasIntersection(&window.boundaries[i], &snake_get_node()->rect)) {
                    window.gameover = true;
                    score_reset();
                    snake_reset();
                }
            }
        }

        SDL_SetRenderDrawColor(window.renderer, 50, 255, 100, 255);
        SDL_RenderClear(window.renderer);

        for (int i = 0; i < sizeof(window.boundaries) / sizeof(SDL_Rect); i++) {
            SDL_SetRenderDrawColor(window.renderer, 155, 155, 155, 155);
            SDL_RenderFillRect(window.renderer, &window.boundaries[i]);
        }

        snake_render(window.renderer);
        apple_render(window.renderer);
        score_render(window.renderer);

        if (window.pause) {
            Text_Render(window.pause_text);
        } else if (window.gameover) {
            Text_Render(window.gameover_text);
            score_reset();
        }

        SDL_RenderPresent(window.renderer);
        SDL_Delay(1000 / 12);
    }
}

void window_quit() {
    SDL_DestroyRenderer(window.renderer);
    SDL_DestroyWindow(window.window);
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
}
