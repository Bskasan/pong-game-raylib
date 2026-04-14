#include "entities.h"
#include "config.h"

// ---------- Ball ----------

void BallInit(Ball *ball)
{
    ball->circle.radius = BALL_RADIUS;
    BallReset(ball);
}

void BallReset(Ball *ball)
{
    ball->circle.x = SCREEN_WIDTH  * 0.5f;
    ball->circle.y = SCREEN_HEIGHT * 0.5f;
    
    const float sx = GetRandomValue(0, 1) ? 1.0f : -1.0f;
    const float sy = GetRandomValue(0, 1) ? 1.0f : -1.0f;
    ball->speed_x = BALL_START_SPEED * sx;
    ball->speed_y = BALL_START_SPEED * sy;
}

bool BallAdvance(Ball *ball, bool *scored_left)
{
    float distance_frame_time = GetFrameTime();
    
    ball->circle.x += ball->speed_x * distance_frame_time;
    ball->circle.y += ball->speed_y * distance_frame_time;
    
    // Top / bottom bounce
    if (ball->circle.y - ball->circle.radius <= 0.0f) 
    {
        ball->circle.y = ball->circle.radius;
        ball->speed_y = -ball->speed_y;
    } 
    else if (ball->circle.y + ball->circle.radius >= SCREEN_HEIGHT) 
    {
        ball->circle.y = SCREEN_HEIGHT - ball->circle.radius;
        ball->speed_y = -ball->speed_y;
    }
    
    // Score: ball off right edge → left player scored
    if (ball->circle.x - ball->circle.radius <= 0.0f) {
        *scored_left = false; // right side scored (ball off left edge)
        return true;
    }
    
    if (ball->circle.x + ball->circle.radius >= SCREEN_WIDTH) {
        *scored_left = true;  // left side scored (ball off right edge)
        return true;
    }
    
    return false;
}

void BallDraw(Ball *ball)
{
    DrawCircle((int)ball->circle.x, (int)ball->circle.y, ball->circle.radius, YELLOW);
}

// ---------- Paddle ----------

Rectangle PaddleRect(Paddle *paddle)
{
    return Rectangle{ paddle->x - paddle->width * 0.5f,
        paddle->y - paddle->heigth * 0.5f,
        paddle->width, paddle->heigth };
}

void PaddleClampToScreen(Paddle *p)
{
    const float half = p->heigth * 0.5f;
    if (p->y - half < 0.0f)              p->y = half;
    if (p->y + half > SCREEN_HEIGHT)     p->y = SCREEN_HEIGHT - half;
}

void PaddleMoveByKeys(Paddle *p, KeyboardKey up, KeyboardKey down)
{
    const float dt = GetFrameTime();
    if (IsKeyDown(up))   p->y -= p->speed * dt;
    if (IsKeyDown(down)) p->y += p->speed * dt;
    
    PaddleClampToScreen(p);
}

void PaddleFollow(Paddle *p, float target_y)
{
    const float dt = GetFrameTime();
    // Simple proportional follow with a small deadzone to avoid jitter.
    const float dy = target_y - p->y;
    const float deadzone = 4.0f;
    if (dy >  deadzone) p->y += p->speed * dt;
    if (dy < -deadzone) p->y -= p->speed * dt;
    
    PaddleClampToScreen(p);
}

void PaddleDraw(Paddle *paddle)
{
    DrawRectangleRounded(PaddleRect(paddle), 0.8f, 0, WHITE);
}