#ifndef PADDLE_H
#define PADDLE_H

#include "game_object.h"

enum PaddleDirection
{
    Up,
    Down,
    stop,
};

class Paddle : public GameObject
{
public:
    Paddle(double left, double top, double width, double height, 
        double velocity);
    // ~Paddle();

    // set direction
    void SetDirection(const PaddleDirection& direction);

    // Typical behavoiur methods
    void Move();

private:
    double width_, height_;     // Paddle size
    double velocity_;           // rate at which the paddle is moving
    PaddleDirection direction_; // up, down or stop
};

#endif