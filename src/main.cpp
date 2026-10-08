#include "raylib.h"

int main()
{
  constexpr int screenWidth{800};
  constexpr int screenHeight{600};

  InitWindow(screenWidth, screenHeight, "Raylib Platformer");
  SetTargetFPS(60);

  /*
    main loop
  */
  while (!WindowShouldClose()) {


    BeginDrawing();
    ClearBackground(BLACK);
    
    EndDrawing();
  }

  CloseWindow();
}
