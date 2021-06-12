#include <iostream>

#include "SDL.h"

#include "paddle.h"
#include "game.h"

Game::Game(const std::size_t max_score)
{
  /**Create game objects**/
  // Create paddles 
  left_paddle_ = std::make_unique<Paddle>(PaddleLocation::LeftPaddle, paddle_width, paddle_height, paddle_velocity);
  right_paddle_ = std::make_unique<Paddle>(PaddleLocation::RightPaddle, paddle_width, paddle_height, paddle_velocity);

  // Create ball
  ball_ = std::make_unique<Ball>(radius, x_velocity, y_velocity);
  /**Create game objects**/

  score_ = Score(max_score); // Move Assignment Operator
}

void Game::Run(Controller & controller, CollisionEngine& collision_engine, Renderer &renderer, 
               std::size_t target_frame_duration) 
{
  Uint32 title_timestamp = SDL_GetTicks();
  Uint32 frame_start;
  Uint32 frame_end;
  Uint32 frame_duration;
  int frame_count = 0;
  int scores[2] {0, 0};
  Winner player;

  /** Launch threads **/
  controller.SetPaddlePointers(left_paddle_.get(), right_paddle_.get());
  threads_.emplace_back(std::thread(&Controller::GetKeyboardInputs, std::ref(controller)));
  
  collision_engine.SetGameObjects(left_paddle_.get(), right_paddle_.get(), ball_.get());
  threads_.emplace_back(std::thread(&CollisionEngine::Run, std::ref(collision_engine)));

  score_.SetCollisionEngine(&collision_engine);
  threads_.emplace_back(std::thread(&Score::UpdateLoop, std::ref(score_)));
  /** Launch threads **/

  // Input, Update, Render - the main game loop.
  while (true) {
    frame_start = SDL_GetTicks();

    // Check if an exit/interrupt signal is received
    SDL_PollEvent(&e);
    if (e.type == SDL_QUIT || (e.type == SDL_WINDOWEVENT && e.window.event == SDL_WINDOWEVENT_CLOSE)) 
      break;
    
    // Update game objects
    Update();

    // Check players' scores
    score_.GetScore(*scores, *(scores+1));
    score_.IsWin(player);
    
    // Render frame
    if (player == Winner::None)
      renderer.Render(left_paddle_.get(), right_paddle_.get(), ball_.get(), scores);
    else
      renderer.Render(player);

    frame_end = SDL_GetTicks();

    // Keep track of how long each loop through the input/update/render cycle
    // takes.
    frame_count++;
    frame_duration = frame_end - frame_start;

    // After every second, update the window title.
    if (frame_end - title_timestamp >= 1000) {
      renderer.UpdateWindowTitle(frame_count);
      frame_count = 0;
      title_timestamp = frame_end;
    }

    // If the time for this frame is too small (i.e. frame_duration is
    // smaller than the target ms_per_frame), delay the loop to
    // achieve the correct frame rate.
    if (frame_duration < target_frame_duration) {
      SDL_Delay(target_frame_duration - frame_duration);
    }
  }

  // Stop the threads
  controller.Stop();
  collision_engine.Stop();
  score_.Stop();

  // Thread barrier: join all threads
  for (auto& t : threads_)
    t.join();
    
}

void Game::Update() {
  left_paddle_->Move();
  right_paddle_->Move();

  // // Check if ball touches left/right edge
  bool should_reset;
  score_.CheckBallOnLeftRight(should_reset);

  if (should_reset)
    ball_->ResetBallPosition();
  ball_->Move();

}