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
#define internal static

global_variable bool play_with_ai = true;

struct Score
{
        int player_score;
        int player_second_score;
        int cpu_score;
};

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
                DrawCircle((int)circle.x, (int)circle.y, circle.radius, YELLOW);
        }
        
        void Move(Score *score)
        {
                circle.x += speed_x * GetFrameTime();
                circle.y += speed_y * GetFrameTime();
                
                // Score
                if(circle.x + circle.radius >= SCREEN_WIDTH)
                {
                        if(play_with_ai)
                                score->cpu_score++;
                        else
                                score->player_second_score++;
                        
                        ResetBall();
                }
                
                if(circle.x - circle.radius <= 0)
                {
                        score->player_score++;
                        ResetBall();
                }
                
                // Top/bottom bounce — keep this
                if(circle.y + circle.radius >= SCREEN_HEIGHT || circle.y - circle.radius <= 0)
                {
                        speed_y *= -1;
                }
        }
        
        void ResetBall()
        {
                circle.x = SCREEN_WIDTH / 2;
                circle.y = SCREEN_HEIGHT / 2;
                
                int speed_choices[2] = {1, -1};
                
                speed_x = 300;
                speed_y = 300;
                
                speed_x *= speed_choices[GetRandomValue(0,1)];
                speed_y *= speed_choices[GetRandomValue(0,1)];
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
                DrawRectangleRounded(GetRect(), 0.8, 0, WHITE);
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

struct GameObjects
{
        Ball ball;
        Paddle right_paddle;
        Paddle left_paddle;
        CPUPaddle cpu_paddle;
};

global_variable Score score_values;
global_variable GameObjects game_entities;


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


internal void
InitGameEntities(GameObjects *game_entities)
{
        game_entities->right_paddle.x = SCREEN_WIDTH - 25;
        game_entities->right_paddle.y = SCREEN_HEIGHT / 2;
        game_entities->right_paddle.width = 30;
        game_entities->right_paddle.height = 150;
        game_entities->right_paddle.speed = 300;
        
        game_entities->left_paddle.x = 25;
        game_entities->left_paddle.y = SCREEN_HEIGHT / 2;
        game_entities->left_paddle.width = 30;
        game_entities->left_paddle.height = 150;
        game_entities->left_paddle.speed = 300;
        
        game_entities->cpu_paddle.paddle.x = 25;
        game_entities->cpu_paddle.paddle.y = SCREEN_HEIGHT / 2;
        game_entities->cpu_paddle.paddle.width  = 30;
        game_entities->cpu_paddle.paddle.height = 150;
        game_entities->cpu_paddle.paddle.speed = 300;
        
        game_entities->ball.circle.radius = 20;
        game_entities->ball.circle.y = SCREEN_HEIGHT / 2;
        game_entities->ball.circle.x = SCREEN_WIDTH / 2;
        game_entities->ball.speed_x = 300;
        game_entities->ball.speed_y = 300;
}

internal void 
RenderGameObjects()
{
        // Clear Background before each frame
        ClearBackground(DARKGRAY);
        DrawRectangle(SCREEN_WIDTH / 2, 0, SCREEN_WIDTH / 2, SCREEN_HEIGHT, GRAY);
        
        DrawCircle(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 150, LIGHTGRAY);
        DrawLine(SCREEN_WIDTH / 2, 0, SCREEN_WIDTH / 2, SCREEN_HEIGHT, WHITE);
        
        game_entities.ball.Draw();
        
        if(play_with_ai)
                game_entities.cpu_paddle.paddle.Draw();
        else
                game_entities.left_paddle.Draw();
        
        game_entities.right_paddle.Draw();
        
        DrawText(TextFormat("%i", play_with_ai ? score_values.cpu_score : score_values.player_second_score), SCREEN_WIDTH / 4 - 20, 20, 80, WHITE);
        DrawText(TextFormat("%i", score_values.player_score), 3 * SCREEN_WIDTH / 4 - 20, 20, 80, WHITE);
}

internal bool
ResolveBallPaddleCollision(Ball *ball, Paddle *paddle)
{
        Rectangle rect = paddle->GetRect();
        
        // Find the nearest point on the paddle rectangle to the ball center
        float nearest_x = clamp(ball->circle.x, rect.x, rect.x + rect.width);
        float nearest_y = clamp(ball->circle.y, rect.y, rect.y + rect.height);
        
        float dx = ball->circle.x - nearest_x;
        float dy = ball->circle.y - nearest_y;
        
        float dist_sq = dx * dx + dy * dy;
        float radius = ball->circle.radius;
        
        // Compare squared distances to avoid the sqrt in the common (no-hit) case
        if(dist_sq >= radius * radius) return false;
        
        if(dist_sq > 0.0f)
        {
                // Normal case: push the ball out along the contact normal
                float dist = square_foot(dist_sq);
                float overlap = radius - dist;
                
                ball->circle.x += (dx / dist) * overlap;
                ball->circle.y += (dy / dist) * overlap;
        }
        
        else
        {
                // Degenerate case: ball center is inside the paddle.
                // Push it out horizontally, opposite to its current travel direction.
                ball->circle.x += (ball->speed_x > 0 ? -radius : radius);
        }
        
        ball->speed_x *= -1.05f;
        return true;
}

internal void
UpdateGame()
{
        
        // Update Section
        game_entities.ball.Move(&score_values);
        game_entities.right_paddle.Move();
        
        Circle circle_object = game_entities.ball.circle;
        Paddle cpu_paddle_for_collision = game_entities.cpu_paddle.paddle;
        
        if(play_with_ai)
        {
                game_entities.cpu_paddle.UpdateAI(game_entities.ball.circle.y);
                ResolveBallPaddleCollision(&game_entities.ball, &game_entities.cpu_paddle.paddle);
        }
        else
        {
                game_entities.left_paddle.Move();
                ResolveBallPaddleCollision(&game_entities.ball, &game_entities.left_paddle);
        }
        
        // left_paddle needs movement too (AI or second player keys)
        // Then check and resolve collisions
        
        ResolveBallPaddleCollision(&game_entities.ball, &game_entities.right_paddle);
        
}

int main(void)
{;
        InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "My Pong Game");
        // Set FPS based on your screen
        SetWindowState(FLAG_VSYNC_HINT);
        
        InitGameEntities(&game_entities);
        
        while(!WindowShouldClose())
        {
                UpdateGame();
                
                // Draw
                BeginDrawing();
                
                RenderGameObjects();
                
                EndDrawing();
        }
        
        CloseWindow();
        
        return 0;
}