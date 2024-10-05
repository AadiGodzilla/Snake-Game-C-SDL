#include "window.h"

int main(int argc, char *argv[]) {
    window_init("Snake Game");
    window_loop();
    window_quit();

    return 0;
}
