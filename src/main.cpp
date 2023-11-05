#include <raylib.h>
#include <game.h>

int main()
{

    InitWindow(1080, 720, "RAYCASTING");

    SetTargetFPS(60);
    
    Setup();
    while (!WindowShouldClose())
    {
        Update();
        BeginDrawing();
        Draw();
        EndDrawing();
    }
    CloseWindow();

    return 0;
}