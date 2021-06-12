/* Author: Shing-Yan Loo (yan99033@gmail.com)
 *
 * Controller clas maps the keyboard controls to their respective actions, i.e.,
 * 'q' and 'a' keys: up and down motion of the left paddle
 * 'p' and 'l' keys: up and down motion of the right paddle
 * 
 */ 

#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <deque>
#include <mutex>
#include <condition_variable>

#include "paddle.h"

class Controller {
public:
  Controller();
  ~Controller();

  // Typical behaviour method 
  void GetKeyboardInputs();

  // Getter/setter
  void SetPaddlePointers(Paddle* left_paddle, Paddle* right_paddle);
  void Stop();

private:
  Paddle* left_paddle_;
  Paddle* right_paddle_;

  bool ShouldQuit();
  
  bool exit_;
  std::mutex exit_mtx_;
};



#endif