#include "game.h"

#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

static int s_getRandomNumer(int lowerBound, int upperBound)
{
    const int numberOfPossibleValues = upperBound - lowerBound + 1;
    return (rand() % numberOfPossibleValues) + lowerBound;
}

static Obstacle_t *s_createObstacle(int width, int height)
{
    Obstacle_t *obstacle = calloc(1, sizeof(Obstacle_t));

    if (obstacle)
    {
        obstacle->posX = width - 1;

        const int gapWidth = height / 3;
        const int minGapCenter = gapWidth + 1;
        const int maxRenderableHorizontalPosition = height - 1;
        const int maxGapCenter = maxRenderableHorizontalPosition - gapWidth - 1;
        const int gapCenter = s_getRandomNumer(minGapCenter, maxGapCenter);
        obstacle->gapTopPosY = gapCenter - gapWidth / 2;
        obstacle->gapBottomPosY = gapCenter + gapWidth / 2;
    }
    return obstacle;
}

void game_init(Game_t *game, int width, int height)
{
    assert(game);
    game->screen = (Screen_t){ .width = width, .height = height };
    game->player = (Player_t){ .posX = 0.1 * width, .posY = height / 2 };
    game->firstObstacle = s_createObstacle(width, height);
    game->lastObstacle = game->firstObstacle;
    game->isGameOver = false;
    srand(time(NULL));
}

static void s_jumpPlayer(Game_t *game)
{
    assert(game);
    const double jumpVelocity = game->screen.height / 100.; //TODO: Figure out something better
    game->player.verticalVelocity = jumpVelocity;
}

static void s_movePlayer(Game_t *game)
{
    assert(game);
    const double velocityChangeDueToGravity = -game->screen.height / 3000.; //TODO: Figure out something better
    game->player.posY -= game->player.verticalVelocity;
    game->player.verticalVelocity += velocityChangeDueToGravity;
}

static bool s_shouldRemoveFirstObstacle(const Game_t *game)
{
    assert(game);
    bool shouldRemove = false;

    if (game->firstObstacle)
    {
        shouldRemove = game->firstObstacle->posX <= 0;
    }
    return shouldRemove;
}

static void s_removeFirstObstacle(Game_t *game)
{
    assert(game);
    if (game->firstObstacle)
    {
        Obstacle_t *secondObstacle = game->firstObstacle->next;
        free(game->firstObstacle);
        game->firstObstacle = secondObstacle;
    }
}

static void s_shiftAllObstacles(Game_t *game)
{
    assert(game);
    Obstacle_t *obstacle = game->firstObstacle;

    while (obstacle)
    {
        const double shiftValue = -0.5;
        obstacle->posX += shiftValue;
        obstacle = obstacle->next;
    }
}

static bool s_shouldCreateNewObstacle(const Game_t *game)
{
    assert(game);
    bool shouldCreate = false;
    const int distanceBetweenObstacles = game->screen.width / 7; //TODO: Figure out something better

    if (game->lastObstacle)
    {
        const int obstacleDistanceToScreenEdge = game->screen.width - 1 - game->lastObstacle->posX;
        if (obstacleDistanceToScreenEdge >= distanceBetweenObstacles)
        {
            shouldCreate = true;
        }
    }
    return shouldCreate;
}

static void s_addNewObstacle(Game_t *game, Obstacle_t *obstacle)
{
    assert(game);
    assert(obstacle);
    game->lastObstacle->next = obstacle;
    game->lastObstacle = obstacle;
}

static bool s_isPlayerCollidedWithObstacle(const Game_t *game)
{
    assert(game);
    bool isCollided = false;

    if (game->firstObstacle)
    {
        const bool isHorizontalMatch = round(game->player.posX) == round(game->firstObstacle->posX);
        const bool isPlayerAboveGap = game->player.posY <= game->firstObstacle->gapTopPosY;
        const bool isPlayerBelowGap = game->player.posY >= game->firstObstacle->gapBottomPosY;
        isCollided = isHorizontalMatch && (isPlayerAboveGap || isPlayerBelowGap);
    }
    return isCollided;
}

static bool s_isPlayerOnTheFloor(const Game_t *game)
{
    assert(game);
    return game->player.posY >= game->screen.height;
}

static bool s_isGameOver(const Game_t *game)
{
    assert(game);
    return s_isPlayerCollidedWithObstacle(game) || s_isPlayerOnTheFloor(game);
}


bool game_tick(Game_t *game, int userInput)
{
    assert(game);
    bool shouldExit = false;

    if (!game->isGameOver)
    {
        if ('q' == userInput)
        {
            shouldExit = true;
        }
        else if (' ' == userInput)
        {
            s_jumpPlayer(game);
        }

        s_movePlayer(game);
        s_shiftAllObstacles(game);

        if (s_shouldRemoveFirstObstacle(game))
        {
             s_removeFirstObstacle(game);
        }
        if (s_shouldCreateNewObstacle(game))
        {
            Obstacle_t *newObstacle = s_createObstacle(game->screen.width, game->screen.height);
            s_addNewObstacle(game, newObstacle);
        }        
        game->isGameOver = s_isGameOver(game);
    }
    else
    {
        if (-1 != userInput)
        {
            shouldExit = true;
        }
    }
    return shouldExit;
}

static void s_renderObstacle(const Game_t *game, const Obstacle_t *obstacle, RenderCharacter_t renderCharacter)
{
    for (int y = 0; y < game->screen.height; ++y)
    {
        if (y < obstacle->gapTopPosY || y > obstacle->gapBottomPosY)
        {
            renderCharacter(obstacle->posX, y, 'O');
        }
    }
}

static void s_renderAllObstacles(const Game_t *game, RenderCharacter_t renderCharacter)
{
    const Obstacle_t *obstacle = game->firstObstacle;

    while (obstacle)
    {
        s_renderObstacle(game, obstacle, renderCharacter);
        obstacle = obstacle->next;
    }
}

static void s_renderPlayer(const Game_t *game, RenderCharacter_t renderCharacter)
{
    renderCharacter(game->player.posX, game->player.posY, '@');
}

static void s_renderGameOverMessage(const Game_t *game, RenderCharacter_t renderCharacter)
{
    const char message[] = "Game Over (press any key to exit)";
    const size_t messageLength = sizeof(message) - 1;
    const int startPosX = game->screen.width / 2 - messageLength / 2;
    const int posY = game->screen.height / 2;

    for (size_t i = 0; i < messageLength; ++i)
    {
        renderCharacter(startPosX + i, posY, message[i]);
    }
}

void game_render(Game_t *game, RenderCharacter_t renderCharacter)
{
    assert(game);
    s_renderAllObstacles(game, renderCharacter);
    s_renderPlayer(game, renderCharacter);

    if (game->isGameOver)
    {
        s_renderGameOverMessage(game, renderCharacter);
    }
}

void s_destroyAllObstacles(Game_t *game)
{
    assert(game);
    while (game->firstObstacle)
    {
        s_removeFirstObstacle(game);
    }
}

void game_deinit(Game_t *game)
{
    assert(game);
    s_destroyAllObstacles(game);
}