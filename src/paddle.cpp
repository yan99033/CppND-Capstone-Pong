#include <assert.h>
#include "paddle.h"

Paddle::Paddle(double left, double top, double width, double height,
    double velocity) : GameObject(left, top, ObjectType::ObjPaddle), width_(width), 
    height_(height), velocity_(velocity)
{
    direction_ = PaddleDirection::Stop;
}

void Paddle::Move()
{
    assert(set_screen_size_);

    if (direction_ == PaddleDirection::Up)
    {
        pos_y_ -= velocity_;

        // Stay in the window
        if (pos_y_ < 0)
            pos_y_ = 0;
    }
    else if (direction_ == PaddleDirection::Down)
    {
        pos_y_ += velocity_;

        if (pos_y_ + height_ > screen_height_)
            pos_y_ = screen_height_ - height_;
    }
}

void Paddle::SetDirection(const PaddleDirection& direction)
{
    direction_ = direction;
}

void Paddle::GetSize(double& width, double& height) const
{
    width = width_;
    height = height_;
}