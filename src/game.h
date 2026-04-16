/* date = April 14th 2026 5:49 pm */
#ifndef PONG_GAME_H
#define PONG_GAME_H

#include "entities.h"

enum Scene
{
    Main,
    Gameplay,
    Paused,
};

enum Mode
{
    VersusAI,
    TwoPlayer,
};

struct Score
{
    int left;   // Left paddle score ( AI or Player 2 )
    int right;  // Right paddle score ( Player 1 )
};


struct GameState
{
    Scene scene;
    Mode mode;
    Score score;
    Ball ball;
    Paddle left_paddle;
    Paddle right_paddle;
};

void GameInit(GameState *game_state);
void GameStartRound(GameState *game_state, Mode mode);  // Reset the game stats
void GameUpdate(GameState *game_state);                 // One frame of gameplay.

#endif //PONG_GAME_H
