#include "renderer_ncurses.h"
#include "input_ncurses.h"
#include "game.h"
#include <unistd.h>

struct Unused_t
{
    int alsoUnused;
    int alsoUnused2;
};


int main()
{
    renderer_init();
    input_init();

    int width = 0;
    int height = 0;
    renderer_getScreenSize(&width, &height);
    
    Game_t game = { 0 };
    game_init(&game, width, height);
    bool shouldStop = false;

    while (!shouldStop)
    {
        int character = input_getCharAsync();
        shouldStop = game_tick(&game, character);
        renderer_clear();
        game_render(&game, renderer_putChar);
        renderer_refresh();
        usleep(16 * 1000);
    }

    game_deinit(&game);
    input_deinit();
    renderer_deinit();

    return 0;
}