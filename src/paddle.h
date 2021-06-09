#ifndef PADDLE_H
#define PADDLE_H

#include "game_object.h"

enum PaddleDirection
{
    Up,
    Down,
    Stop,
};

class Paddle : public GameObject
{
public:
    Paddle(double left, double top, double width, double height, 
        double velocity);
    // ~Paddle();

    // set direction
    void SetDirection(const PaddleDirection& direction);
    void GetSize(double& width, double& height) const;

    // Typical behavoiur methods
    void Move();

private:
    double width_, height_;     // Paddle size
    double velocity_;           // rate at which the paddle is moving
    PaddleDirection direction_; // up, down or stop
};

typedef std::shared_ptr<Paddle> PaddlePtr;

#endif