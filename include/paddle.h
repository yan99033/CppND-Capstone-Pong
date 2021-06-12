/* Author: Shing-Yan Loo (yan99033@gmail.com)
 *
 * Paddle class contains the GameObject attributes as well as its oen
 * set of attributes (paddle size and velocity) and method
 * 
 * The Move method is overridden from the base class to make the 
 * paddle move vertically within the window
 */ 

#ifndef PADDLE_H
#define PADDLE_H

#include <mutex>

#include "game_object.h"

enum class PaddleDirection
{
    Up,
    Down,
    Stop,
};

class Paddle : public GameObject
{
public:
    Paddle(PaddleLocation location, size_t width, size_t height, size_t velocity);
    // ~Paddle();

    // Getter/setter
    void SetDirection(const PaddleDirection& direction);
    void GetPaddleSize(int& width, int& height) const;

    // Typical behaviour methods
    void Move();
    bool CollidePoint(const int& x, const int& y) const;

private:
    PaddleLocation location_;   // left/right paddle
    size_t width_, height_;     // Paddle size
    size_t velocity_;           // rate at which the paddle is moving
    PaddleDirection direction_; // up, down or stop
    std::mutex mutex_;
};

typedef std::unique_ptr<Paddle> PaddlePtr;

#endif