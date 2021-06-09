/* Author: Shing-Yan Loo (yan99033@gmail.com)
 *
 * GameObject class is created as a base class for paddle and ball objects
 * The derived classes can be found in paddle.h/cpp and ball.h/cpp
 */

#ifndef GAME_OBJECT
#define GAME_OBJECT

#include <vector>
#include <memory>

enum ObjectType
{
    ObjPaddle,
    ObjBall,
};

// Abstract class
class GameObject
{
public:
    GameObject(const double pos_x, const double pos_y, const ObjectType obj_type);
    // ~GameObject();

    // typical behaviour methods
    virtual void Move() = 0; // pure virtual

    // getter/setter
    static void SetScreenSize(size_t width, size_t height);
    void SetPosition(double x, double y);
    void GetPosition(double& x, double& y) const;
    ObjectType GetType() const;

protected:
    ObjectType obj_type_;   // class type
    double pos_x_, pos_y_;   // location of the object

    // to ensure game objects are inside the window
    static bool set_screen_size_;       // true if the screen size is set; else the game is not going to work
    static std::size_t screen_width_;
    static std::size_t screen_height_;

};

typedef std::shared_ptr<GameObject> GameObjectPtr;

// class Ball : public GameObject
// {
// public:
//     Ball(const double x, const double y, const double radius);
//     ~Ball();

// private:
//     double radius;
// };


#endif