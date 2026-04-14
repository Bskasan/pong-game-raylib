/* date = April 14th 2026 5:48 pm */
#ifndef PONG_COLLISION_H
#define PONG_COLLISION_H

#include "entities.h"

// NOTE: Resolves circle-vs-rectangle collision between the ball and paddle.
// On hit: pushes the ball out along the contact normal and reflects
// its x velocity, applying BALL_SPEED_GAIN up to BALL_MAX_SPEED.
// Returns true if a collision was resolved this call.
bool ResolveBallPaddleCollision(Ball *ball, Paddle *paddle);

#endif // PONG_COLLISION_H