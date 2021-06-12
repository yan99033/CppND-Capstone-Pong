#include "score.h"

#include <assert.h>
#include <iostream>

Score::Score(int max_score) : max_score_(max_score), left_score_(0), 
    right_score_(0), exit_(false), side_(Touch::None)
{
    assert(max_score >= 0);
}

Score::~Score()
{
    Stop();

    collision_engine_ = nullptr;
}

Score::Score(const Score& other)
{
    std::cout << "Score Copy Constructor" << std::endl;

    max_score_ = other.max_score_;
    left_score_ = other.left_score_;
    right_score_ = other.right_score_;
    exit_ = other.exit_;
    side_ = other.side_;

    if (!other.collision_engine_)
        collision_engine_ = other.collision_engine_;
}

Score &Score::operator=(const Score& other)
{
    std::cout << "Score Copy Assignment Operator" << std::endl;

    if (this != &other)
    {
        max_score_ = other.max_score_;
        left_score_ = other.left_score_;
        right_score_ = other.right_score_;
        exit_ = other.exit_;
        side_ = other.side_;

        if (!other.collision_engine_)
            collision_engine_ = other.collision_engine_;
    }
    
    return *this;
}

Score::Score(Score&& other)
{
    std::cout << "Score Move Constructor" << std::endl;
    max_score_ = other.max_score_;
    left_score_ = other.left_score_;
    right_score_ = other.right_score_;
    exit_ = other.exit_;
    side_ = other.side_;

    other.max_score_ = 0;
    other.left_score_ = 0;
    other.right_score_ = 0;
    other.exit_ = false;
    other.side_ = Touch::None;

    if (!other.collision_engine_)
    {
        // Transfer the pointer and nullify the other one
        collision_engine_ = other.collision_engine_;
        other.collision_engine_ = nullptr;
    }
}

Score &Score::operator=(Score&& other)
{
    std::cout << "Score Move Assignment Operator" << std::endl;

    if (this != &other)
    {
        max_score_ = other.max_score_;
        left_score_ = other.left_score_;
        right_score_ = other.right_score_;
        exit_ = other.exit_;

        other.max_score_ = 0;
        other.left_score_ = 0;
        other.right_score_ = 0;
        other.exit_ = false;
        other.side_ = Touch::None;

        if (!other.collision_engine_)
        {
            // Transfer the pointer and nullify the other one
            collision_engine_ = other.collision_engine_;
            other.collision_engine_ = nullptr;
        }
    }

    return *this;
}

void Score::ResetScore()
{
    std::lock_guard<std::mutex> lck(score_mtx_);
    left_score_ = 0;
    right_score_ = 0;
}

void Score::SetCollisionEngine(CollisionEngine* collision_engine)
{
    collision_engine_ = collision_engine;
}

void Score::GetScore(int& left_score, int& right_score)
{
    std::lock_guard<std::mutex> lck(score_mtx_);
    
    left_score = left_score_;
    right_score = right_score_;
}

void Score::CheckBallOnLeftRight(bool& touch)
{
    std::lock_guard<std::mutex> lck(side_mtx_);
    if (side_ != Touch::None)
    {
        // Always default to None 
        // As we only use it to update the score and ball location once 
        side_ = Touch::None;
        touch = true;
    }
    else
        touch = false;
}

void Score::Stop()
{
    std::lock_guard<std::mutex> lck(exit_mtx_);
    exit_ = true;
}

void Score::IsWin(Winner& who_won)
{
    std::lock_guard<std::mutex> lck(score_mtx_);

    if (left_score_ >= max_score_)
        who_won = Winner::Player1;
    else if (right_score_ >= max_score_)
        who_won = Winner::Player2;
    else
        who_won = Winner::None;
}


void Score::UpdateLoop()
{
    assert(collision_engine_);

    std::cout << "Started thread: Score::UpdateLoop\n";

    while (!ShouldQuit())
    {
        // only execute the subsequent lines if a signal is received
        Touch side = collision_engine_->WaitForTouchSignal();
        
        {
            std::lock_guard<std::mutex> lck(side_mtx_);
            side_ = std::move(side);
        }

        std::lock_guard<std::mutex> lck(score_mtx_);
        if (side_ == Touch::Left)
            right_score_ += 1;
        else if (side_ == Touch::Right)
            left_score_ += 1;
    }

    std::cout << "Exited thread: Score::UpdateLoop\n";
}


bool Score::ShouldQuit()
{
    std::lock_guard<std::mutex> lck(exit_mtx_);
    return exit_;
}