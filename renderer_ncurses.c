#include "renderer_ncurses.h"

#include <ncurses.h>

typedef struct
{
    void *res;
} Renderer_t;

bool ncursesInitialized;

int renderer_init(void)
{
    if (!ncursesInitialized)
    {
        initscr();
        ncursesInitialized = true;
    }
    return 0;
}

void renderer_getScreenSize(int *width, int *height)
{
    getmaxyx(stdscr, *height, *width);
}

static bool s_isPointOnScreen(int posX, int posY)
{
    int width = 0;
    int height = 0;
    renderer_getScreenSize(&width, &height);
    return posX >= 0 && posX < width && posY >= 0 && posY < height;
}

void renderer_putChar(int posX, int posY, char character)
{
    if (s_isPointOnScreen(posX, posY))
    {
        move(posY, posX);
        addch(character);
    }
}

void renderer_clear(void)
{
    clear();
}

void renderer_refresh(void)
{
    refresh();
}

void renderer_deinit(void)
{
    if (ncursesInitialized)
    {
        endwin();
        ncursesInitialized = false;
    }
}