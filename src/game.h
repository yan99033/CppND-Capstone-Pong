#ifndef GAME_H
#define GAME_H

#include <memory>
#include <thread>

#include <random>
#include "SDL.h"
#include "controller.h"
#include "renderer.h"
#include "snake.h"
#include "game_object.h"

// class Paddle;
class Game {
 public:
  Game(std::size_t grid_width, std::size_t grid_height);
  void Run(Controller const &controller, Controller2& controller2, Renderer &renderer,
           std::size_t target_frame_duration);
  int GetScore() const;
  int GetSize() const;

 private:
  Snake snake;
  SDL_Point food;

  SDL_Event e;

  // std::vector<std::unique_ptr<GameObject>> game_objects_;
  PaddlePtr left_paddle_;
  PaddlePtr right_paddle_;

  int left_score_;
  int right_score_;

  std::random_device dev;
  std::mt19937 engine;
  std::uniform_int_distribution<int> random_w;
  std::uniform_int_distribution<int> random_h;

  int score{0};

  

  void PlaceFood();
  void Update();

  // Create threads and destroy upon exit
  std::vector<std::thread> threads_;
};

#endif