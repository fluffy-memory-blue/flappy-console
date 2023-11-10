#ifndef PLAYER_H_
#define PLAYER_H

#include <stdbool.h>

typedef struct
{
    int width;
    int height;
} Screen_t;

typedef struct
{
    double posX;
    double posY;
    double verticalVelocity;
} Player_t;

typedef struct Obstacle_t
{
    double posX;
    double gapTopPosY;
    double gapBottomPosY;
    struct Obstacle_t *next;
} Obstacle_t;

typedef struct
{
    Screen_t screen;
    Player_t player;
    Obstacle_t *firstObstacle;
    Obstacle_t *lastObstacle;
    bool isGameOver;
} Game_t;

typedef void (*RenderCharacter_t)(int posX, int posY, char character);

void game_init(Game_t *game, int width, int height);

bool game_tick(Game_t *game, int userInput);

void game_render(Game_t *game, RenderCharacter_t renderCharacter);

void game_deinit(Game_t *game);

#endif