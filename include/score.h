/* Author: Shing-Yan Loo (yan99033@gmail.com) 
 * 
 * Keep track of players' score
 * when the ball hits the left edge, right player gets a point;
 * wheb the ball hits the right edge, left player gets a point
 * 
 * The Game checks if either player gets max score, and end the 
 * game accordingly. 
 * 
 */

#ifndef SCORE_H
#define SCORE_H

#include <mutex>

#include "collision_engine.h"

enum class Winner
{
    None,
    Player1,
    Player2,
};

class Score
{
public:
    Score(int max_score);
    Score() {}
    ~Score();

    // Copy constructor/assignment
    Score(const Score& other);
    Score &operator=(const Score& other);

    // Move constructor/assignment
    Score(Score&& other);
    Score &operator=(Score&& other);


    // Getter/setter
    void ResetScore();
    void SetCollisionEngine(CollisionEngine* collision_engine);
    void GetScore(int& left_score, int& right_score);
    void CheckBallOnLeftRight(bool& touch);
    void Stop();
    void IsWin(Winner& who_won);

    // Typical behaviour method
    void UpdateLoop();

private:
    CollisionEngine* collision_engine_;

    int max_score_;
    int left_score_;
    int right_score_;
    std::mutex score_mtx_;

    bool ShouldQuit();
  
    bool exit_;
    std::mutex exit_mtx_;

    Touch side_;
    std::mutex side_mtx_;
};

#endif