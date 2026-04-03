#include <stdio.h>
#include "raylib.h"

// --------------------------------------------
// Game Structure
// ---------------
// Definitions
// - Defining variables needed
// - Creating the game objects
// Game Loop
// - Updating the positions of the game objects
// - Checking for the collisions
// --------------------------------------------

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 800

#define global_variable static

global_variable bool play_with_ai = true;

struct Circle
{
        float x, y;
        float radius;
};

struct Ball
{
        Circle circle;
        float speed_x, speed_y;
        
        void Draw()
        {
                DrawCircle((int)circle.x, (int)circle.y, circle.radius, WHITE);
        }
        
        void Move()
        {
                circle.x += speed_x * GetFrameTime();
                circle.y += speed_y * GetFrameTime();
                
                // Top/bottom bounce — keep this
                if(circle.y + circle.radius >= SCREEN_HEIGHT || circle.y - circle.radius <= 0)
                {
                        speed_y *= -1;
                }
        }
};

struct Paddle
{
        float x, y;
        float speed;
        float width, height;
        
        Rectangle GetRect()
        {
                return Rectangle{x - width / 2, y - height / 2, width, height};
        }
        
        void Draw()
        {
                DrawRectangleRec(GetRect(), WHITE);
        }
        
        void LimitMovement()
        {
                if(y - height / 2 <= 0)
                {
                        y = height / 2;
                }
                
                if(y + height / 2 >= SCREEN_HEIGHT)
                {
                        y = SCREEN_HEIGHT - height / 2;
                }
        }
        
        void Move()
        {
                if(IsKeyDown(KEY_UP))
                {
                        y -= speed * GetFrameTime();
                }
                
                if(IsKeyDown(KEY_DOWN))
                {
                        y += speed * GetFrameTime();
                }
                
                LimitMovement();
        }
};


struct CPUPaddle
{
        Paddle paddle;
        
        void UpdateAI(int ball_y)
        {
                if(paddle.y + paddle.height / 2 > ball_y)
                {
                        paddle.y -= paddle.speed * GetFrameTime();
                }
                
                if(paddle.y + paddle.height / 2 <= ball_y)
                {
                        paddle.y += paddle.speed * GetFrameTime();
                }
                
                paddle.LimitMovement();
        }
};

float clamp(float value, float min, float max)
{
        if(value < min) return min;
        if(value > max) return max;
        return value;
}

float square_foot(float val)
{
	int left = 0, right = val, mid;
	float sqrt;
	while (left <= right)	//finding the integral part 
	{
                
		mid = (left + right) / 2;
		if (mid *mid == val)
		{
			sqrt = mid;
			break;
		}
		if (mid * mid < val)
		{
			sqrt = left;
			left = mid + 1;
		}
		else
		{
			right = mid - 1;
		}
	}
        
	float inc_val = 0.1;
	for (int i = 0; i < 3; i++)	///finding the fractional part
	{
		while (sqrt * sqrt <= val)
		{
			sqrt += inc_val;
		}
		sqrt = sqrt - inc_val;
		inc_val = inc_val / 10;
	}
	return sqrt;
}


bool collision_check_for_circle(Circle* circle, Paddle* paddle)
{
        Rectangle rect = paddle->GetRect();
        
        float nearest_x = clamp(circle->x, rect.x, rect.x + rect.width);
        float nearest_y = clamp(circle->y, rect.y, rect.y + rect.height);
        
        float distance_x = circle->x - nearest_x;
        float distance_y = circle->y - nearest_y;
        
        float distance = square_foot(distance_x * distance_x + distance_y * distance_y);
        float overlap = circle->radius - distance;
        
        
        if(overlap > 0.0f)
        {
                return true;
        }
        
        return false;
}


int main(void)
{;
        InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "My Pong Game");
        // Set FPS based on your screen
        SetWindowState(FLAG_VSYNC_HINT);
        
        // Update Display 60 times per second with this.
        // TOP-LEFT is the 0,0 for our game as position
        
        Ball ball = {};
        
        Paddle right_paddle = {};
        Paddle left_paddle = {};
        CPUPaddle cpu_paddle = {};
        
        right_paddle.x = SCREEN_WIDTH - 25;
        right_paddle.y = SCREEN_HEIGHT / 2;
        right_paddle.width = 30;
        right_paddle.height = 150;
        right_paddle.speed = 300;
        
        left_paddle.x = 25;
        left_paddle.y = SCREEN_HEIGHT / 2;
        left_paddle.width = 30;
        left_paddle.height = 150;
        left_paddle.speed = 300;
        
        cpu_paddle.paddle.x = 25;
        cpu_paddle.paddle.y = SCREEN_HEIGHT / 2;
        cpu_paddle.paddle.width  = 30;
        cpu_paddle.paddle.height = 150;
        cpu_paddle.paddle.speed = 300;
        
        ball.circle.radius = 20;
        ball.circle.y = SCREEN_HEIGHT / 2;
        ball.circle.x = SCREEN_WIDTH / 2;
        ball.speed_x = 300;
        ball.speed_y = 300;
        
        
        while(!WindowShouldClose())
        {
                // Update Section
                
                // Animate the Ball  
                
                ball.Move();
                right_paddle.Move();
                
                Circle circle_object = ball.circle;
                Paddle cpu_paddle_for_collision = cpu_paddle.paddle;
                
                if(play_with_ai)
                {
                        cpu_paddle.UpdateAI(ball.circle.y);
                        
                        if(collision_check_for_circle(&circle_object, &cpu_paddle_for_collision))
                        {
                                Rectangle rect = cpu_paddle_for_collision.GetRect();
                                
                                // Finds the closest point
                                float nearest_x = clamp(circle_object.x, rect.x, rect.x + rect.width);
                                float nearest_y = clamp(circle_object.y, rect.y, rect.y + rect.height);
                                
                                float dx = circle_object.x - nearest_x;
                                float dy = circle_object.y - nearest_y;
                                
                                float distance = square_foot(dx * dx + dy * dy);
                                
                                float overlap = circle_object.radius - distance;
                                
                                if(distance > 0.0f)
                                {
                                        circle_object.x += (dx / distance) * overlap;
                                        circle_object.y += (dy / distance) * overlap;
                                }
                                
                                // resolve overlap, then bounce
                                ball.speed_x *= -1.05f;
                                ball.circle = circle_object;
                        }
                }
                else
                {
                        left_paddle.Move();
                        
                        if(collision_check_for_circle(&circle_object, &left_paddle))
                        {
                                Rectangle rect = left_paddle.GetRect();
                                
                                float nearest_x = clamp(circle_object.x, rect.x, rect.x + rect.width);
                                float nearest_y = clamp(circle_object.y, rect.y, rect.y + rect.height);
                                
                                float dx = circle_object.x - nearest_x;
                                float dy = circle_object.y - nearest_y;
                                
                                float distance = square_foot(dx * dx + dy * dy);
                                
                                float overlap = circle_object.radius - distance;
                                
                                if(distance > 0.0f)
                                {
                                        circle_object.x += (dx / distance) * overlap;
                                        circle_object.y += (dy / distance) * overlap;
                                }
                                
                                // resolve overlap, then bounce
                                ball.speed_x *= -1.05f;
                                ball.circle = circle_object;
                        }
                }
                
                // left_paddle needs movement too (AI or second player keys)
                // Then check and resolve collisions
                
                if(collision_check_for_circle(&circle_object, &right_paddle))
                {
                        Rectangle rect = right_paddle.GetRect();
                        
                        float nearest_x = clamp(circle_object.x, rect.x, rect.x + rect.width);
                        float nearest_y = clamp(circle_object.y, rect.y, rect.y + rect.height);
                        
                        float dx = circle_object.x - nearest_x;
                        float dy = circle_object.y - nearest_y;
                        
                        float distance = square_foot(dx * dx + dy * dy);
                        float overlap = circle_object.radius - distance;
                        
                        if(distance > 0.0f)
                        {
                                circle_object.x += (dx / distance) * overlap;
                                circle_object.y += (dy / distance) * overlap;
                        }
                        
                        // resolve overlap, then bounce
                        ball.speed_x *= -1.05f;
                        ball.circle = circle_object;
                }
                
                
                // Draw
                BeginDrawing();
                
                // Clear Background before each frame
                ClearBackground(BLACK);
                
                // TODO(bekir): Refactoring neeeded for game arena elements.
                DrawLine(SCREEN_WIDTH / 2, 0, SCREEN_WIDTH / 2, SCREEN_HEIGHT, WHITE);
                
                ball.Draw();
                
                if(play_with_ai)
                        cpu_paddle.paddle.Draw();
                else
                        left_paddle.Draw();
                
                right_paddle.Draw();
                
                EndDrawing();
        }
        
        CloseWindow();
        
        return 0;
}