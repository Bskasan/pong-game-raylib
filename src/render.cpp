#include "render.h"
#include "config.h"

#include "raylib.h"

static void DrawCenteredText(const char* text, int y, int font_size, Color color)
{
    int text_width = MeasureText(text, font_size);
    DrawText(text, (SCREEN_WIDTH - text_width) / 2, y, font_size, color);
}

void RenderMenu()
{
    ClearBackground(GRAY);
    
    int center_height = SCREEN_HEIGHT / 2;
    
    DrawCenteredText("Welcome to the Pong Game!", 
                     center_height - MENU_Y_OFFSET,
                     MENU_FONT_SIZE,
                     LIGHTGRAY);
    DrawCenteredText("Press [Enter] to play vs AI", 
                     center_height,
                     MENU_FONT_SIZE,
                     LIGHTGRAY);
    DrawCenteredText("Press [P] to play 2-player local", 
                     center_height + MENU_Y_OFFSET,
                     MENU_FONT_SIZE,
                     LIGHTGRAY);
    DrawCenteredText("Press [Esc] to quit!", 
                     center_height + MENU_Y_OFFSET * 2,
                     MENU_FONT_SIZE,
                     LIGHTGRAY);
}

void RenderPauseMenu()
{
    ClearBackground(GRAY);
    
    int center_height = SCREEN_HEIGHT / 2;
    
    DrawCenteredText("Paused",
                     center_height - MENU_Y_OFFSET * 2, 
                     MENU_FONT_SIZE, 
                     LIGHTGRAY);
    DrawCenteredText("Press [P] to resume",
                     center_height, 
                     MENU_FONT_SIZE, 
                     LIGHTGRAY);
    DrawCenteredText("Press [Esc] for main menu",
                     center_height + MENU_Y_OFFSET, 
                     MENU_FONT_SIZE, 
                     LIGHTGRAY);
}


void RenderGamePlay(GameState *game_state)
{
    // Draw Playground
    ClearBackground(DARKGRAY);
    
    DrawRectangle(SCREEN_WIDTH / 2, 0, SCREEN_WIDTH / 2, SCREEN_HEIGHT, GRAY);
    DrawCircle(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 150, LIGHTGRAY);
    DrawLine(SCREEN_WIDTH / 2, 0, SCREEN_WIDTH / 2, SCREEN_HEIGHT, WHITE);
    
    // Entities
    PaddleDraw(&game_state->left_paddle);
    PaddleDraw(&game_state->right_paddle);
    BallDraw(&game_state->ball);
    
    // Score
    int score_y = 20;
    
    DrawText(TextFormat("%i", game_state->score.left), 
             SCREEN_WIDTH / 4 - 20, score_y, 
             SCORE_FONT_SIZE, 
             WHITE);
    DrawText(TextFormat("%i", game_state->score.right), 
             3 * SCREEN_WIDTH / 4 - 20, score_y, 
             SCORE_FONT_SIZE,
             WHITE);
}
