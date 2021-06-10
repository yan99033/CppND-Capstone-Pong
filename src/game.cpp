#include "game.h"
#include <iostream>
#include "SDL.h"

#include "paddle.h"


Game::Game(std::size_t grid_width, std::size_t grid_height)
    : snake(grid_width, grid_height),
      engine(dev()),
      random_w(0, static_cast<int>(grid_width - 1)),
      random_h(0, static_cast<int>(grid_height - 1)) {
  PlaceFood();

  /**Create game objects**/
  // Create paddles 
  int paddle_width = 25;
  int paddle_height = 200;
  int paddle_velocity = 10;
  left_paddle_ = std::make_shared<Paddle>(ObjectType::LeftPaddle, paddle_width, paddle_height, paddle_velocity);
  right_paddle_ = std::make_shared<Paddle>(ObjectType::RightPaddle, paddle_width, paddle_height, paddle_velocity);

  // Create ball

  /**Create game objects**/

}

void Game::Run(Controller const &controller, Controller2 & controller2, Renderer &renderer,
               std::size_t target_frame_duration) {
  Uint32 title_timestamp = SDL_GetTicks();
  Uint32 frame_start;
  Uint32 frame_end;
  Uint32 frame_duration;
  int frame_count = 0;
  bool running = true;

  controller2.SetPaddleObjects(left_paddle_, right_paddle_);
  threads_.emplace_back(std::thread(&Controller2::GetKeyboardInputs, std::ref(controller2)));
  // std::thread t = std::thread(&Controller2::GetKeyboardInputs, std::ref(controller2));

  // Input, Update, Render - the main game loop.
  while (running) {
    frame_start = SDL_GetTicks();

    // Check if an exit signal is received
    SDL_PollEvent(&e);
    if (e.type == SDL_QUIT || (e.type == SDL_WINDOWEVENT && e.window.event == SDL_WINDOWEVENT_CLOSE)) {
      // exit the while-loop and kill all the threads
      controller2.Stop();
      running = false;
    }
    
    Update();
    
    renderer.Render(snake, food, left_paddle_, right_paddle_);

    frame_end = SDL_GetTicks();

    // Keep track of how long each loop through the input/update/render cycle
    // takes.
    frame_count++;
    frame_duration = frame_end - frame_start;

    // After every second, update the window title.
    if (frame_end - title_timestamp >= 1000) {
      renderer.UpdateWindowTitle(score, frame_count);
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

  // t.join();
  // join all threads
  for (auto& t : threads_)
    t.join();
}

void Game::PlaceFood() {
  int x, y;
  while (true) {
    x = random_w(engine);
    y = random_h(engine);
    // Check that the location is not occupied by a snake item before placing
    // food.
    if (!snake.SnakeCell(x, y)) {
      food.x = x;
      food.y = y;
      return;
    }
  }
}

void Game::Update() {
  if (!snake.alive) return;

  left_paddle_->Move();
  right_paddle_->Move();

  snake.Update();

  int new_x = static_cast<int>(snake.head_x);
  int new_y = static_cast<int>(snake.head_y);

  // Check if there's food over here
  if (food.x == new_x && food.y == new_y) {
    score++;
    PlaceFood();
    // Grow snake and increase speed.
    snake.GrowBody();
    snake.speed += 0.02;
  }
}

int Game::GetScore() const { return score; }
int Game::GetSize() const { return snake.size; }