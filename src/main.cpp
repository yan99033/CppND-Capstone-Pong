#include <iostream>

#include "controller.h"
#include "collision_engine.h"
#include "score.h"
#include "game.h"
#include "renderer.h"

int main() {
  constexpr std::size_t kFramesPerSecond{60};
  constexpr std::size_t kMsPerFrame{1000 / kFramesPerSecond};
  constexpr std::size_t kScreenWidth{640};
  constexpr std::size_t kScreenHeight{640};
  constexpr std::size_t kFontSize{50};
  constexpr std::size_t kMaxScore{10};

  // Set screen size
  GameObject::SetScreenSize(kScreenWidth, kScreenHeight);

  Renderer renderer(kScreenWidth, kScreenHeight);
  Controller controller;
  CollisionEngine collision_engine(kScreenWidth, kScreenHeight);
  
  Game game(kMaxScore);
  game.Run(controller, collision_engine, renderer, kMsPerFrame);
  std::cout << "Game has terminated successfully!\n";
  return 0;
}