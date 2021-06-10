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
    LeftPaddle,
    RightPaddle,
    Ball,
};

// Abstract class
class GameObject
{
public:
    // GameObject();
    // ~GameObject();

    // typical behaviour methods
    virtual void Move() = 0; // pure virtual

    // getter/setter
    static void SetScreenSize(int width, int height);
    static void SetPaddleOffset(int offset);
    void SetPosition(int x, int y);
    void GetPosition(int& x, int& y) const;
    // ObjectType GetType() const;

protected:
    // ObjectType obj_type_;   // class type

    // location of the object
    int pos_x_;
    int pos_y_;   

    // to ensure game objects are inside the window
    static bool set_screen_size_;       // true if the screen size is set; else the game is not going to work
    static int screen_width_;
    static int screen_height_;
    static int paddle_offset_;

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