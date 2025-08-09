#ifndef __CIRCLE_HPP__
#define __CIRCLE_HPP__

#include <xspacewar.hpp>

class Circle {
private:
    Vector2 center;
    float radius;
public:
    Circle(const Vector2 &center, float radius);
    bool overlaps(const Circle &other);
};

#endif //__CIRCLE_HPP__