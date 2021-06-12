/* Author: Shing-Yan Loo (yan99033@gmail.com)
 * 
 * The main game loop:
 * - input (controller)
 * - update
 * - render (renderer)
 * 
 * Credit: Thanks to the starter code provided by Udacity
 * (https://github.com/udacity/CppND-Capstone-Snake-Game)
 */ 

#ifndef GAME_H
#define GAME_H

#include <memory>
#include <thread>
#include <random>

#include "SDL.h"

#include "controller.h"
#include "collision_engine.h"
#include "renderer.h"
#include "game_object.h"
#include "paddle.h"
#include "ball.h"
#include "score.h"

// TODO: make the paddle and ball attributes scale according to the window size
// Paddle attributes
constexpr int paddle_width = 20;
constexpr int paddle_height = 150;
constexpr int paddle_velocity = 10;

// Ball Attributes 
// Note: Keep in mind that the x_velocity mustn't be greater than the paddle_width
//        or else the step increment would 'shoot' past the paddles
constexpr int radius = 10;
constexpr int x_velocity = 5;
constexpr int y_velocity = 2;

// class Paddle;
class Game {
 public:
  Game(const std::size_t max_score);

  // Typical behaviour methods
  void Run(Controller& controller, CollisionEngine& collision_engine, Renderer &renderer,
           std::size_t target_frame_duration);

 private:
  SDL_Event e;

  PaddlePtr left_paddle_;
  PaddlePtr right_paddle_;
  BallPtr ball_;

  Score score_;

  void Update();

  // Create threads and destroy upon exit
  std::vector<std::thread> threads_;
};

#endif