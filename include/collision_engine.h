/* Author: Shing-Yan Loo (yan99033@gmail.com)
 *
 * Collision detection between the ball and the inner side of the paddles
 * and between the objects (paddles and ball) and the window edges 
 * 
 * When the ball hits the inner side of the paddles, the ball reflects horizontally;
 * when it hits the top/bottom side of the window, it reflects vertically
 * 
 * When the ball hits the left/right side of the window, the players' score
 * increases accordingly
 */ 

#ifndef COLLISION_ENGINE_H
#define COLLISION_ENGINE_H

#include <deque>
#include <condition_variable>

#include "paddle.h"
#include "ball.h"

enum class Touch
{
    None,
    Left,
    Right,
};

template <class T>
class SignalQueue
{
public:
    void Send(T &&msg);
    void Stop();
    bool ShouldStop();
    T Receive();

private:
    std::deque<T> queue_;
    std::condition_variable condition_;
    std::mutex mutex_;

    bool exit_ {false};
    std::mutex blk_mutex_;
};

typedef SignalQueue<Touch> TouchSignal;

class CollisionEngine
{
public:
    CollisionEngine(const std::size_t screen_width, const std::size_t screen_height);
    ~CollisionEngine();

    // Getter/setter
    void SetGameObjects(Paddle* left_paddle, Paddle* right_paddle, Ball* ball);
    void Stop();

    // Typical behaviour methods
    Touch WaitForTouchSignal();
    void Run();


private:
    Paddle* left_paddle_;
    Paddle* right_paddle_;
    Ball* ball_;

    bool ShouldQuit();

    bool exit_;
    std::mutex exit_mutex_;

    const int screen_width_;
    const int screen_height_;

    TouchSignal signal_;

};

#endif