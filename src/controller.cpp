#include "controller.h"
#include <iostream>
#include "SDL.h"
#include "snake.h"
#include <thread>
#include <cassert>

void Controller::ChangeDirection(Snake &snake, Snake::Direction input,
                                 Snake::Direction opposite) const {
  if (snake.direction != opposite || snake.size == 1) snake.direction = input;
  return;
}

void Controller::HandleInput(bool &running, Snake &snake) const {
  SDL_Event e;
  while (SDL_PollEvent(&e)) {
    if (e.type == SDL_QUIT) {
      running = false;
    } else if (e.type == SDL_KEYDOWN) {
      switch (e.key.keysym.sym) {
        case SDLK_UP:
          ChangeDirection(snake, Snake::Direction::kUp,
                          Snake::Direction::kDown);
          break;

        case SDLK_DOWN:
          ChangeDirection(snake, Snake::Direction::kDown,
                          Snake::Direction::kUp);
          break;

        case SDLK_LEFT:
          ChangeDirection(snake, Snake::Direction::kLeft,
                          Snake::Direction::kRight);
          break;

        case SDLK_RIGHT:
          ChangeDirection(snake, Snake::Direction::kRight,
                          Snake::Direction::kLeft);
          break;
      }
    }
  }
}

template <typename T>
T ControlSignalQueue<T>::Receive()
{
    // FP.5a : The method receive should use std::unique_lock<std::mutex> and _condition.wait() 
    // to wait for and receive new messages and pull them from the queue using move semantics. 
    // The received object should then be returned by the receive function. 
//   
  	std::unique_lock<std::mutex> lck(mutex_);
    condition_.wait(lck, [this] { return !queue_.empty(); });

    // Remove the first element in the queue (FIFO)
    T msg = std::move(queue_.front());
    queue_.pop_front();    

    return msg; // will not be copied due to return value optimization in C++

}

template <typename T>
void ControlSignalQueue<T>::Send(T &&msg)
{
    // FP.4a : The method send should use the mechanisms std::lock_guard<std::mutex> 
    // as well as _condition.notify_one() to add a new message to the queue and afterwards send a notification.
  
  	std::lock_guard <std::mutex> lck(mutex_);
    queue_.push_back(std::move(msg));

    condition_.notify_one();
}

Controller2::Controller2() : exit_(false)
{}

Controller2::~Controller2()
{
  Stop();
}

void Controller2::Stop()
{
  std::lock_guard<std::mutex> lck(exit_mtx_);
  std::cout << "Stop: " << exit_ << std::endl; 
  exit_ = true;
}

bool Controller2::ShouldQuit()
{
  std::lock_guard<std::mutex> lck(exit_mtx_);
  return exit_;
}

/* An infinite while-loop running in a separate thread
is used to detect key presses so that we can modify the 
location of the paddles accordingly
Get key presses without using events
https://stackoverflow.com/questions/17580166/sdl-get-keyboard-state-without-using-events
 */
void Controller2::GetKeyboardInputs()
{
  // make sure the paddles are set
  assert(left_paddle_ != nullptr);
  assert(right_paddle_ != nullptr);

  std::cout << "Started keyboard input thread\n";

  
  const uint8_t* keys = SDL_GetKeyboardState(NULL);
  while (!ShouldQuit()) {
    // Update the keyboard state
    SDL_PumpEvents();

    // if both up and down keys are pressed (cancel out)
    if (keys[SDL_SCANCODE_Q] && !keys[SDL_SCANCODE_A])
    {
      left_paddle_->SetDirection(PaddleDirection::Up);
      // std::cout << "Q key detected \n"; 
    }
    else if (keys[SDL_SCANCODE_A] && !keys[SDL_SCANCODE_Q])
    {
      left_paddle_->SetDirection(PaddleDirection::Down);
      // std::cout << "A key detected \n"; 
    }
    else
    {
      left_paddle_->SetDirection(PaddleDirection::Stop); 
      // std::cout << "no key detected\n";
    }
    if (keys[SDL_SCANCODE_P] && !keys[SDL_SCANCODE_L])
    {
      right_paddle_->SetDirection(PaddleDirection::Up);
      // std::cout << "P key detected \n";
    }
    else if (keys[SDL_SCANCODE_L] && !keys[SDL_SCANCODE_P])
    {
      right_paddle_->SetDirection(PaddleDirection::Down);
      // std::cout << "L key detected \n";
    }
    else
    {
      right_paddle_->SetDirection(PaddleDirection::Stop);
      // std::cout << "No key detected \n";
    }

    // Free CPU
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
  }

  std::cout << "Exited keyboard input thread\n";
}

void Controller2::SetPaddleObjects(PaddlePtr& left_paddle, PaddlePtr& right_paddle)
{
  left_paddle_ = left_paddle;
  right_paddle_ = right_paddle;
}