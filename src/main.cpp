#include <memory>
#include <vector>
#include <unordered_map>
#include <iostream>
#include "raylib.h"

enum class PlayerState {
  standing = 0,
  running = 1,
  jumping = 2
};

class Player
{
public:
  Player() {}
  ~Player() { cleanup(); }
  bool init() {
    Texture2D tex;
    std::unordered_map<PlayerState, Texture2D> new_map;
    tex = LoadTexture("assets/standing.png");
    if (!IsTextureValid(tex)) {
      cleanup();
      return false;
    }
    new_map[PlayerState::standing] = tex;
    tex = LoadTexture("assets/run.png");
    if (!IsTextureValid(tex)) {
      cleanup();
      return false;
    }
    new_map[PlayerState::running] = tex;
    tex = LoadTexture("assets/jump.png");
    if (!IsTextureValid(tex)) {
      cleanup();
      return false;
    }
    new_map[PlayerState::jumping] = tex;
    playerTex.push_back(new_map);
    
    return true;
  }

  void update() {
    
  }

  Texture2D getTexture(PlayerState state) {
    return playerTex[static_cast<int>(state)][state];
  }

  void render() {
    DrawTexture(getTexture(currentState), 400, 300, WHITE);
    
  }

  void cleanup() {
    if(!playerTex.empty()) {
      for (size_t i{}; i < playerTex.size(); ++i) {
	for (auto& [key, value] : playerTex[i]){
	  std::cout << "unloading " << static_cast<int>(key) << std::endl;
	  UnloadTexture(value);
	}
      }
    }
  }
private:

  std::vector<std::unordered_map<PlayerState, Texture2D>> playerTex;
  PlayerState currentState = PlayerState::standing;
};

class Game
{
public:
  Game(){
    InitWindow(windowWidth, windowHeight, "Raylib Platformer");
    SetTargetFPS(60);
    if (!player.init()) {
      cleanup();
    }
  }
  ~Game() { cleanup(); }
  void run() {
    while (!WindowShouldClose()) {
      update();
      render();
    }
  }
private:
  void update() {
    player.update();
  }

  void render() {
    BeginDrawing();
    ClearBackground(BLACK);
    player.render();
    EndDrawing();
  }

  void cleanup() {
    CloseWindow();
  }

  Player player;
  const int windowWidth = 800;
  const int windowHeight = 600;
};

int main()
{
  std::unique_ptr<Game> game = std::make_unique<Game>();
  game->run();
}
