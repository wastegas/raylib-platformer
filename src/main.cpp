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
    std::unordered_map<PlayerState, TexInfo> new_map1, new_map2, new_map3;
    tex = LoadTexture("assets/standing.png");
    if (!IsTextureValid(tex)) {
      cleanup();
      return false;
    }
    new_map1[PlayerState::standing] = {tex, tex.width, tex.height, 303, 2};
    playerTex.push_back(new_map1);
    tex = LoadTexture("assets/run.png");
    if (!IsTextureValid(tex)) {
      cleanup();
      return false;
    }
    new_map2[PlayerState::running] = {tex, tex.width, tex.height, 307, 5};
    playerTex.push_back(new_map2);
    tex = LoadTexture("assets/jump.png");
    if (!IsTextureValid(tex)) {
      cleanup();
      return false;
    }
    new_map3[PlayerState::jumping] = {tex, tex.width, tex.height, 329 , 2};
    playerTex.push_back(new_map3);

    #ifdef DEBUG_MODE
    std::cout << "playerTex size=" << playerTex.size() << std::endl;
    #endif

    getTexture(currentState);
        
    return true;
  }

  void update(float &dt) {
    float speed = 200.f;

    if (IsKeyDown(KEY_D)) {
      destPos.x += speed * dt;
      currentState = PlayerState::running;
      getTexture(currentState);
      #ifdef DEBUG_MODE
      std::cout << "currentState=" << static_cast<int>(currentState) << std::endl;
      #endif
      #ifdef DEBUG_MODE
      if (IsTextureValid(currentTexture)) 
	std::cout << "texture running loaded" << std::endl;
      else
	std::cout << "texture running not loaded" << std::endl;
      std::cout << "destPos.x = " << destPos.x << std::endl;
      #endif
    }
    if (IsKeyDown(KEY_A)) {
      destPos.x -= speed * dt;
      currentState = PlayerState::running;
      getTexture(currentState);
      sourceRec.width = -sourceRec.width;

    }
    if (IsKeyDown(KEY_SPACE)) {
      destPos.y += jumpForce *dt;
      currentState = PlayerState::jumping;
      getTexture(currentState);
      gravity = 150.f;
      grounded = false;
    }
    if (IsKeyReleased(KEY_D) || IsKeyReleased(KEY_A) || IsKeyReleased(KEY_SPACE)) {
      speed = 0.f;
      currentState = PlayerState::standing;
      getTexture(currentState);
      #ifdef DEBUG_MODE
      std::cout << "currentState=" << static_cast<int>(currentState) << std::endl;
      #endif
    }
    
    runningTime += dt;
    if (runningTime >= frameTime) {
      runningTime = 0.f;
      currentFrame = ((int)currentFrame +1) % totalFrames;
      sourceRec.x = (float)currentFrame * frameWidth;
    }
    //velY += gravity;
    destPos.y += gravity * dt;
  }

  void getTexture(PlayerState state) {
    TexInfo t = playerTex[static_cast<int>(state)][state];
    totalFrames = t.numFrames;
    frameWidth = t.frameWidth;
    sourceRec = {0.f, 0.f, static_cast<float>(t.frameWidth), static_cast<float>(t.height)};
    currentTexture =  t.texture;
  }

  Rectangle getHitbox() {
    return Rectangle{destPos.x, destPos.y, static_cast<int>(currentTexture.width), static_cast<int>(currentTexture.height)};
  }

  void resetPlayerY(int currY) {
    if (!grounded) {
      destPos.y -= 9;
      velY = 0.f;
      grounded = true;
      gravity = 0.f;
    }
  }

  bool checkGrounded() {
    return grounded;
  }
  void render() {
    #ifndef DEBUG_MODE
    DrawTextureRec(currentTexture, sourceRec, destPos, WHITE);
    #else
    DrawTexture(currentTexture, 400, 300, WHITE);
    #endif
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
  float currentFrame;
  Texture2D currentTexture;
  float frameTime = 0.01f;
  int frameWidth = 0;
  float runningTime = 0.f;
  Rectangle sourceRec = {0.f, 0.f, 0.f, 0.f};
  Vector2 destPos = {400.f, 300.f};
  int totalFrames = 0;
  float jumpForce = -400.f;
  float gravity = 50.1f;
  float velY = 0.f;
  bool grounded = false;
};

class Game
{
public:
  Game(){
    #ifdef DEBUG_MODE
    SetTraceLogLevel(LOG_NONE);
    #endif
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
      float dt = GetFrameTime();
      update(dt);
      render();
    }
  }
private:
  void update(float &dt) {
    player.update(dt);
    if (!player.checkGrounded()) {
      if (CheckCollisionRecs(player.getHitbox(), floor)) {
#ifdef DEBUG_MODE
	std::cout << "collision!" << std::endl;
#endif
	player.resetPlayerY(floor.y);
      }
    }
  }

  void render() {
    BeginDrawing();
    ClearBackground(BLACK);
    player.render();
    DrawRectangle(floor.x, floor.y, floor.width, floor.height, RAYWHITE);
    EndDrawing();
  }

  void cleanup() {
    player.cleanup();
    CloseWindow();
    //std::cout << "Game cleanup" << std::endl;
  }

  Player player;
  const int windowWidth = 1024;
  const int windowHeight = 768;
  Rectangle floor = {0.f, 758, 1024.f, 10.f};

};

int main()
{
  std::unique_ptr<Game> game = std::make_unique<Game>();
  game->run();
}
