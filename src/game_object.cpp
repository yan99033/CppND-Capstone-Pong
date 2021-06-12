#include "game_object.h"
#include <cassert>

// Static variable initialization
bool GameObject::set_screen_size_ = false;
int GameObject::screen_width_ = 0;
int GameObject::screen_height_ = 0;
int GameObject::paddle_offset_ = 60;

/* abstract GameObject class */
// GameObject::GameObject()
// {}



// GameObject::~GameObject()       
// {
//     // remove all game objects
//     game_objects_.clear();
// }


void GameObject::SetScreenSize(int width, int height)
{
    screen_width_ = width;
    screen_height_ = height;
    set_screen_size_ = true;
}

void GameObject::SetPaddleOffset(int offset)
{
    paddle_offset_ = offset;
}

void GameObject::GetPosition(int& x, int& y)
{
    std::lock_guard<std::mutex> lck(pos_mutex_);
    x = pos_x_;
    y = pos_y_;
}

void GameObject::SetPosition(int x, int y)
{
    std::lock_guard<std::mutex> lck(pos_mutex_);
    pos_x_ = x;
    pos_y_ = y;
}


