#ifndef PADDLE_H
#define PADDLE_H

#include <mutex>

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
    Paddle(ObjectType obj, size_t width, size_t height, size_t velocity);
    // ~Paddle();

    // set direction
    void SetDirection(const PaddleDirection& direction);
    void GetSize(int& width, int& height) const;

    // Typical behavoiur methods
    void Move();

private:
    size_t width_, height_;     // Paddle size
    size_t velocity_;           // rate at which the paddle is moving
    PaddleDirection direction_; // up, down or stop
    std::mutex mutex_;
};

typedef std::shared_ptr<Paddle> PaddlePtr;

#endif