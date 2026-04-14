#include "raylib.h"

#include "config.h"
#include "game.h"
#include "render.h"

static void UpdateScene(GameState *game_state)
{
    switch(game_state->scene)
    {
        case Scene::Main:
        {
            if(IsKeyPressed(KEY_ENTER)) 
                GameStartRound(game_state, Mode::VersusAI);
            
            if(IsKeyPressed(KEY_P)) 
                GameStartRound(game_state, Mode::TwoPlayer);
        }break;
        
        case Scene::Gameplay:
        {
            if(IsKeyPressed(KEY_SPACE)) 
            {
                game_state->scene = Scene::Paused;
            }
            
            GameUpdate(game_state);
        }break;
        
        case Scene::Paused:
        {
            if(IsKeyPressed(KEY_P)) 
            {
                game_state->scene = Scene::Gameplay;
            }
            else if (IsKeyPressed(KEY_ESCAPE))
            {
                game_state->scene = Scene::Main;
            }
        }break;
    }
}


static void DrawScene(GameState *game_state)
{
    switch(game_state->scene)
    {
        case Scene::Main:
        {
            RenderMenu();
        }break;
        
        case Scene::Paused:
        {
            RenderPauseMenu();
        }break;
        
        case Scene::Gameplay:
        {
            RenderGamePlay(game_state);
        }break;
    }
}


int main(int argc, char* argv[])
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_TITLE);
    SetWindowState(FLAG_VSYNC_HINT);
    SetExitKey(KEY_NULL);
    
    // Initialize game state.
    GameState game_state = {};
    GameInit(&game_state);
    
    while(!WindowShouldClose())
    {
        if(game_state.scene == Scene::Main && IsKeyPressed(KEY_ESCAPE))
            break;
        
        UpdateScene(&game_state);
        
        BeginDrawing();
        DrawScene(&game_state);
        EndDrawing();
        
    }
    
    CloseWindow();
    
    return (0);
}