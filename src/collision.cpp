#include "collision.h"
#include "config.h"

#include <math.h>

static float clampf(float value, float lowest, float highest)
{
    if (value < lowest) return lowest;
    if (value > highest) return highest;
    return value;
}

bool ResolveBallPaddleCollision(Ball *ball, Paddle *paddle)
{
    const Rectangle rect = PaddleRect(paddle);
    
    // Closest point on the rectangle to the ball center.
    const float nearest_x = clampf(ball->circle.x, rect.x, rect.x + rect.width);
    const float nearest_y = clampf(ball->circle.y, rect.y, rect.y + rect.height);
    
    const float distance_x = ball->circle.x - nearest_x;
    const float distance_y = ball->circle.y - nearest_y;
    
    const float distance_sqr = distance_x * distance_x + distance_y * distance_y;
    const float radius  = ball->circle.radius;
    
    // Squared-distance check avoids a sqrt in the common (no-hit) case.
    if (distance_sqr >= radius * radius) return false;
    
    if (distance_sqr > 0.0f) {
        // Push the ball out along the contact normal.
        const float dist    = sqrtf(distance_sqr);
        const float overlap = radius - dist;
        
        ball->circle.x += (distance_x / dist) * overlap;
        ball->circle.y += (distance_y / dist) * overlap;
    } else {
        // Ball center exactly inside the paddle — push it out horizontally,
        // opposite to its travel direction.
        ball->circle.x += (ball->speed_x > 0.0f ? -radius : radius);
    }
    
    // Reflect X velocity and scale up, but clamp to BALL_MAX_SPEED.
    ball->speed_x = -ball->speed_x * BALL_SPEED_GAIN;
    ball->speed_y =  ball->speed_y * BALL_SPEED_GAIN;
    ball->speed_x = clampf(ball->speed_x, -BALL_MAX_SPEED, BALL_MAX_SPEED);
    ball->speed_y = clampf(ball->speed_y, -BALL_MAX_SPEED, BALL_MAX_SPEED);
    
    return true;
}