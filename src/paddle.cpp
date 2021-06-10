#include <assert.h>
#include "paddle.h"

Paddle::Paddle(ObjectType obj, size_t width, size_t height, size_t velocity) : 
    width_(width), height_(height), velocity_(velocity)
{
    assert(set_screen_size_);

    if (obj == ObjectType::LeftPaddle)
    {
        pos_x_ = paddle_offset_;
        pos_y_ = static_cast<size_t>((screen_height_ - height) / 2);
    }
    else if (obj == ObjectType::RightPaddle)
    {
        pos_x_ = screen_width_ - paddle_offset_ - width;
        pos_y_ = static_cast<size_t>((screen_height_ - height) / 2);
    }
    else
    {
        throw std::invalid_argument("unknown ObjectType");
    }
    

    direction_ = PaddleDirection::Stop;
}

void Paddle::Move()
{
    // Make sure the screen size is set 
    // so that paddle stays in the window
    assert(set_screen_size_);

    std::lock_guard<std::mutex> lck(mutex_);
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
    std::lock_guard<std::mutex> lck(mutex_);
    direction_ = direction;
}

void Paddle::GetSize(int& width, int& height) const
{
    width = width_;
    height = height_;
}