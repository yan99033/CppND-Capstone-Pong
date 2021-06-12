#include "collision_engine.h"

#include <assert.h>
#include <thread>
#include <iostream>

template <typename T>
T SignalQueue<T>::Receive()
{  
  	std::unique_lock<std::mutex> lck(mutex_);
    condition_.wait(lck, [this] { return !queue_.empty() || ShouldStop(); });

    if (queue_.empty())
        return T();

    // Remove the first element in the queue (FIFO)
    T msg = std::move(queue_.front());
    queue_.pop_front();    

    return msg; // will not be copied due to return value optimization in C++

}

template <typename T>
void SignalQueue<T>::Stop()
{
    // Unblock the condition variable
    std::lock_guard<std::mutex> lck(blk_mutex_);
    exit_ = true;
    condition_.notify_one();
}

template <typename T>
bool SignalQueue<T>::ShouldStop()
{
    std::lock_guard<std::mutex> lck(blk_mutex_);
    return exit_;
}

template <typename T>
void SignalQueue<T>::Send(T &&msg)
{
  	std::lock_guard <std::mutex> lck(mutex_);
    queue_.push_back(std::move(msg));

    condition_.notify_one();
}

CollisionEngine::CollisionEngine(const std::size_t screen_width, const std::size_t screen_height) :
    screen_width_(screen_width), screen_height_(screen_height), exit_(false)
{}

CollisionEngine::~CollisionEngine()
{
    Stop();

    left_paddle_ = nullptr;
    right_paddle_ = nullptr;
    ball_ = nullptr;
}

void CollisionEngine::SetGameObjects(Paddle* left_paddle, Paddle* right_paddle, Ball* ball)
{
    left_paddle_ = left_paddle;
    right_paddle_ = right_paddle;
    ball_ = ball;
}

void CollisionEngine::Stop()
{
    // Block signal from being queued
    signal_.Stop();

    std::lock_guard<std::mutex> lck(exit_mutex_);
    exit_ = true;
}

bool CollisionEngine::ShouldQuit()
{
    std::lock_guard<std::mutex> lck(exit_mutex_);
    return exit_;
}

Touch CollisionEngine::WaitForTouchSignal()
{
    return std::move(signal_.Receive()); 
}

void CollisionEngine::Run()
{
    assert(left_paddle_ || right_paddle_ || ball_);

    std::cout << "Started thread: CollisionEngine::Run\n";

    int ball_x;
    int ball_y;
    int ball_radius;
    Direction ball_direction;

    // Assume ball size won't change
    ball_->GetBallRadius(ball_radius);

    while (!ShouldQuit())
    {
        // Get ball position
        ball_->GetPosition(ball_x, ball_y);
        ball_direction = ball_->GetBallDirection();

        // Check if ball hits the window edges
        if ((ball_x - ball_radius < 0) && 
            (ball_direction == Direction::LeftUp || ball_direction == Direction::LeftDown))
            ball_->SetBounceDirection(Bounce::Horizontal);
        if ((ball_x + ball_radius >= screen_width_) && 
            (ball_direction == Direction::RightUp || ball_direction == Direction::RightDown))
            ball_->SetBounceDirection(Bounce::Horizontal);
        if ((ball_y - ball_radius < 0) &&
            (ball_direction == Direction::LeftUp || ball_direction == Direction::RightUp))
            ball_->SetBounceDirection(Bounce::Vertical);
        if ((ball_y + ball_radius >= screen_height_) &&
            (ball_direction == Direction::LeftDown || ball_direction == Direction::RightDown))
            ball_->SetBounceDirection(Bounce::Vertical);

        // Check if ball hits the inner side of paddles
        if ((left_paddle_->CollidePoint(ball_x - ball_radius, ball_y)) && 
            (ball_direction == Direction::LeftUp || ball_direction == Direction::LeftDown))
            ball_->SetBounceDirection(Bounce::Horizontal);
        else if ((right_paddle_->CollidePoint(ball_x + ball_radius, ball_y)) && 
            (ball_direction == Direction::RightUp || ball_direction == Direction::RightDown))
            ball_->SetBounceDirection(Bounce::Horizontal);

        // Check if ball hits the left/right window edges (send a signal to increase the corresponding score)
        if ((ball_x - ball_radius < 0) && 
            (ball_direction == Direction::LeftUp || ball_direction == Direction::LeftDown))
            signal_.Send(Touch::Left);
        else if ((ball_x + ball_radius >= screen_width_) && 
            (ball_direction == Direction::RightUp || ball_direction == Direction::RightDown))
            signal_.Send(Touch::Right);

        // Free CPU
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    std::cout << "Exited thread: CollisionEngine::Run\n";
}

