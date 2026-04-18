#include "game.h"
#include "collision.h"
#include "config.h"


void InitPaddle(Paddle *paddle, float x)
{
    paddle->x      = x;
    paddle->y      = SCREEN_HEIGHT / 0.5f;
    paddle->width  = PADDLE_WIDTH;
    paddle->heigth = PADDLE_HEIGHT;
    paddle->speed  = PADDLE_SPEED;
}

void GameInit(GameState *game_state)
{
    game_state->scene = Scene::Main;
    game_state->mode  = Mode::VersusAI;
    game_state->score = {
        0,
        0,
    };
    
    InitPaddle(&game_state->left_paddle, PADDLE_MARGIN);
    InitPaddle(&game_state->right_paddle, SCREEN_WIDTH - PADDLE_MARGIN);
    BallInit(&game_state->ball);
}

void GameStartRound(GameState *game_state, Mode mode)
{
    game_state->mode           = mode;
    game_state->scene          = Scene::Gameplay;
    game_state->score          = { 0, 0 };
    game_state->left_paddle.y  = SCREEN_HEIGHT * 0.5f;
    game_state->right_paddle.y = SCREEN_HEIGHT * 0.5f;

    BallReset(&game_state->ball);
}

void GameUpdate(GameState *game_state)
{
    // Player 1 - Movement setup
    PaddleMoveByKeys(&game_state->right_paddle, KEY_UP, KEY_DOWN);
    
    // Player 2 ( with W / S ) or AI
    if(game_state->mode == Mode::VersusAI)
    {
        PaddleFollow(&game_state->left_paddle, game_state->ball.circle.y);
    }
    else
    {
        PaddleMoveByKeys(&game_state->left_paddle, KEY_W, KEY_S);
    }
    
    // Handle Scoring
    bool scored_left = false;
    if(BallAdvance(&game_state->ball, &scored_left))
    {
        if(scored_left) game_state->score.left++;
        else game_state->score.right++;
        
        BallReset(&game_state->ball);
        return;
    }
    
    // Resolve paddle collisions with ball
    // A ball can't hit both paddles in the same frame at same speed.
    if(!ResolveBallPaddleCollision(&game_state->ball, &game_state->left_paddle))
    {
        ResolveBallPaddleCollision(&game_state->ball, &game_state->right_paddle);
    }
}
