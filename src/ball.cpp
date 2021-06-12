#include "ball.h"
#include <cassert>

Ball::Ball(int radius, int x_velocity, int y_velocity) :
    radius_(radius), x_velocity_(x_velocity), y_velocity_(y_velocity)
{
    pos_x_ = static_cast<int>(screen_width_ / 2);
    pos_y_ = static_cast<int>(screen_height_ / 2);
}

void Ball::SetBounceDirection(const Bounce& direction)
{
    std::lock_guard<std::mutex> lock(vel_mutex_);

    if (direction == Bounce::Vertical)
        y_velocity_ = -y_velocity_;
    else if (direction == Bounce::Horizontal)
        x_velocity_ = -x_velocity_;
}

void Ball::GetBallRadius(int& radius) const
{
    radius = radius_;
}

Direction Ball::GetBallDirection()
{
    std::lock_guard<std::mutex> lock(vel_mutex_);

    if (x_velocity_ > 0 && y_velocity_ < 0)
        return Direction::RightUp;
    else if (x_velocity_ > 0 && y_velocity_ > 0)
        return Direction::RightDown;
    else if (x_velocity_ < 0 && y_velocity_ < 0)
        return Direction::LeftUp;
    else if (x_velocity_ < 0 && y_velocity_ > 0)
        return Direction::LeftDown;
    else
        return Direction::Undefined;
}

void Ball::ResetBallPosition()
{
    int pos_x = static_cast<int>(screen_width_ / 2);
    int pos_y = static_cast<int>(screen_height_ / 2);

    SetPosition(pos_x, pos_y);
}


// Collision (window and paddle) is being handled outside
void Ball::Move()
{
    std::lock_guard<std::mutex> lock(vel_mutex_);
    std::lock_guard<std::mutex> lock2(pos_mutex_);

    pos_x_ += x_velocity_;
    pos_y_ += y_velocity_;
}