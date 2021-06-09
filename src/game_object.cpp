#include "game_object.h"

/* abstract GameObject class */
GameObject::GameObject(const double pos_x, const double pos_y, const ObjectType obj_type) :
    pos_x_(pos_x), pos_y_(pos_y), obj_type_(obj_type)
{
    
}

// Static variable definition
bool GameObject::set_screen_size_ = false;
size_t GameObject::screen_width_ = 0;
size_t GameObject::screen_height_ = 0;

// GameObject::~GameObject()       
// {
//     // remove all game objects
//     game_objects_.clear();
// }


void GameObject::SetScreenSize(size_t width, size_t height)
{
    screen_width_ = width;
    screen_height_ = height;
    set_screen_size_ = true;
}

void GameObject::GetPosition(double& x, double& y) const
{
    x = pos_x_;
    y = pos_y_;
}

void GameObject::SetPosition(double x, double y)
{
    pos_x_ = x;
    pos_y_ = y;
}

ObjectType GameObject::GetType() const
{
    return obj_type_;
}


