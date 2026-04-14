/* date = April 14th 2026 5:37 pm */

#ifndef PONG_ENTITIES_H
#define PONG_ENTITIES_H

#include "raylib.h"

struct Circle
{
    float x, y;
    float radius;
};

struct Ball
{
    Circle circle;
    float speed_x, speed_y;
};

struct Paddle
{
    float x, y;
    float width, heigth;
    float speed;
};

// Ball
void BallInit(Ball *ball);
void BallReset(Ball *ball);

// NOTE: Advances the ball; returns true if a point was scored.
// If scored, 'scored_left' is set; true fi the Left side scored.
bool BallAdvance(Ball *ball, bool *scored_left);
void BallDraw(Ball *ball);

// Paddle
Rectangle PaddleRect(Paddle *paddle);
void PaddleClampToScreen(Paddle *paddle);
void PaddleMoveByKeys(Paddle *paddle, KeyboardKey up, KeyboardKey down);
void PaddleFollow(Paddle *paddle, float target_y);
void PaddleDraw(Paddle *paddle);

#endif //PONG_ENTITIES_H
