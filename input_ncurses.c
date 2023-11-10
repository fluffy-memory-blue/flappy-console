#include "input_ncurses.h"

#include <ncurses.h>

extern bool ncursesInitialized;

int input_init(void)
{
    if (!ncursesInitialized)
    {
        initscr();
        nodelay(stdscr, true);
        ncursesInitialized = true;
    }
    else
    {
        nodelay(stdscr, true);
    }

    return 0;
}

int input_getCharAsync(void)
{
    return getch();
}


void input_deinit(void)
{
    if (ncursesInitialized)
    {
        endwin();
        ncursesInitialized = false;
    }
}