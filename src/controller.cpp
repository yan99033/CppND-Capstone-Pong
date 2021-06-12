#include "controller.h"
#include "SDL.h"

#include <iostream>
#include <thread>
#include <cassert>

Controller::Controller() : exit_(false)
{}

Controller::~Controller()
{
  Stop();
}

void Controller::Stop()
{
  std::lock_guard<std::mutex> lck(exit_mtx_);
  exit_ = true;
}

bool Controller::ShouldQuit()
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
void Controller::GetKeyboardInputs()
{
  // make sure the paddles are set
  assert(left_paddle_ != nullptr);
  assert(right_paddle_ != nullptr);

  std::cout << "Started thread: Controller::GetKeyboardInputs\n";

  const uint8_t* keys = SDL_GetKeyboardState(NULL);
  while (!ShouldQuit()) {
    // Update the keyboard state
    SDL_PumpEvents();

    // if both up and down keys are pressed, then no response will
    // be given (cancel out)

    // Player 1 control ('q' and 'a' keys)
    if (keys[SDL_SCANCODE_Q] && !keys[SDL_SCANCODE_A])
      left_paddle_->SetDirection(PaddleDirection::Up);
    else if (keys[SDL_SCANCODE_A] && !keys[SDL_SCANCODE_Q])
      left_paddle_->SetDirection(PaddleDirection::Down);
    else
      left_paddle_->SetDirection(PaddleDirection::Stop); 

    // Player 2 control ('p' and 'l' keys)
    if (keys[SDL_SCANCODE_P] && !keys[SDL_SCANCODE_L])
      right_paddle_->SetDirection(PaddleDirection::Up);
    else if (keys[SDL_SCANCODE_L] && !keys[SDL_SCANCODE_P])
      right_paddle_->SetDirection(PaddleDirection::Down);
    else
      right_paddle_->SetDirection(PaddleDirection::Stop);

    // Free CPU
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
  }

  std::cout << "Exited thread: Controller::GetKeyboardInputs\n";
}

void Controller::SetPaddlePointers(Paddle* left_paddle, Paddle* right_paddle)
{
  // Make sure both pointers point to valid memory addresses
  assert(left_paddle && right_paddle);

  left_paddle_ = left_paddle;
  right_paddle_ = right_paddle;
}