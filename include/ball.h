/* Author: Shing-Yan Loo (yan99033@gmail.com)
 *
 * Ball class contains the GameObject attributes as well as its own 
 * set of attributes (radius and velocity vector) and methods.
 * 
 * The Move method is overridden from the base class to make the 
 * ball move within the window
 */ 


#ifndef BALL_H
#define BALL_H

#include <mutex>
#include "game_object.h"

enum class Bounce
{
    Horizontal,
    Vertical,
};

enum class Direction
{
    LeftUp,
    LeftDown,
    RightUp,
    RightDown,
    Undefined,
};

class Ball : public GameObject
{
public:
    Ball(int radius, int x_velocity, int y_velocity);

    // Getter/setter
    void SetBounceDirection(const Bounce& direction);
    void GetBallRadius(int& radius) const;
    Direction GetBallDirection();
    void ResetBallPosition();

    // Typical behaviour methods
    void Move();

private:
    int radius_;
    int x_velocity_;
    int y_velocity_;
    std::mutex vel_mutex_;
};

typedef std::unique_ptr<Ball> BallPtr;


#endif