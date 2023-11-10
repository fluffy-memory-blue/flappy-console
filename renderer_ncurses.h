#ifndef RENDERER_H_
#define RENDERER_H_

#include <stdlib.h>

int renderer_init(void);

void renderer_getScreenSize(int *width, int *height);

void renderer_putChar(int posX, int posY, char character);

void renderer_clear(void);

void renderer_refresh(void);

void renderer_deinit(void);

#endif
