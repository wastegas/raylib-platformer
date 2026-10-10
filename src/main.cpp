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

// sprite info
struct TexInfo {
  Texture2D texture;
  int width;
  int height;
  int frameWidth;
  int numFrames;
};

class Player
{
public:
  Player() {}
  ~Player() { cleanup(); }
  bool init() {
    Texture2D tex;
    std::unordered_map<PlayerState, TexInfo> new_map;
    tex = LoadTexture("assets/standing.png");
    if (!IsTextureValid(tex)) {
      cleanup();
      return false;
    }
    new_map[PlayerState::standing] = {tex, tex.width, tex.height, 303, 2};
    tex = LoadTexture("assets/run.png");
    if (!IsTextureValid(tex)) {
      cleanup();
      return false;
    }
    new_map[PlayerState::running] = {tex, tex.width, tex.height, 307, 5};
    tex = LoadTexture("assets/jump.png");
    if (!IsTextureValid(tex)) {
      cleanup();
      return false;
    }
    new_map[PlayerState::jumping] = {tex, tex.width, tex.height, 329 , 2};
    playerTex.push_back(new_map);
    
    return true;
  }

  void update() {
    
  }

  Texture2D getTexture(PlayerState state) {
    TexInfo t = playerTex[static_cast<int>(state)][state];
    return t.texture;
  }

  void render() {
    DrawTexture(getTexture(currentState), 400, 300, WHITE);
    
  }

  void cleanup() {
    if(!playerTex.empty()) {
      for (size_t i{}; i < playerTex.size(); ++i) {
	for (auto& [key, value] : playerTex[i]){
	  //std::cout << "unloading " << static_cast<int>(key) << std::endl;
	  if (IsTextureValid(value.texture))
	    UnloadTexture(value.texture);
	}
	playerTex.clear(); // prevent cleanup from running twice
      }
    }
    //std::cout << "player destructor" << std::endl;
  }
private:

  std::vector<std::unordered_map<PlayerState, TexInfo>> playerTex;
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
  ~Game() {
    cleanup();
    //std::cout << "game destructor" << std::endl;
  }
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
    player.cleanup();
    CloseWindow();
    //std::cout << "Game cleanup" << std::endl;
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
