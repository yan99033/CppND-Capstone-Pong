#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <deque>
#include <mutex>
#include <condition_variable>

#include "paddle.h"


#include "snake.h"

class Controller {
 public:
  void HandleInput(bool &running, Snake &snake) const;

 private:
  void ChangeDirection(Snake &snake, Snake::Direction input,
                       Snake::Direction opposite) const;
};

// class 
template <class T>
class ControlSignalQueue
{
public:
    void Send(T &&msg);
    T Receive();

private:
    std::deque<T> queue_;
    std::condition_variable condition_;
    std::mutex mutex_;

};

class Controller2 {
public:
  // Constructor
  Controller2(PaddlePtr& left_paddle, PaddlePtr& right_paddle);

  // Typical behaviour method 
  void GetKeyboardInputs();
  
  
  // PaddleDirection GetLeftPaddleControl();
  // PaddleDirection GetRightPaddleControl();

private:
  PaddlePtr left_paddle_;
  PaddlePtr right_paddle_;

  // // keyboard control signals
  // ControlSignalQueue<PaddleDirection> left_signal_;
  // ControlSignalQueue<PaddleDirection> right_signal_;
};



#endif