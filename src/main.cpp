#include <memory>
#include "raylib.h"

class Game
{
public:
  Game(){
    InitWindow(windowWidth, windowHeight, "Raylib Platformer");
    SetTargetFPS(60);
  }
  ~Game() { CloseWindow(); }
  void run() {
    while (!WindowShouldClose()) {
      update();
      render();
    }
  }
private:
  void update() {
    
  }

  void render() {
    BeginDrawing();
    ClearBackground(BLACK);

    EndDrawing();
  }
  
  const int windowWidth = 800;
  const int windowHeight = 600;
};

int main()
{
  std::unique_ptr<Game> game = std::make_unique<Game>();
  game->run();
}
