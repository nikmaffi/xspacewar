#ifndef __CIRCLE_HPP__
#define __CIRCLE_HPP__

#include <xspacewar.hpp>

struct Circle {
    Vector2 center;
    float radius;

    Circle(const Vector2 &center, float radius);
    bool overlaps(const Circle &other) const;
};

#endif //__CIRCLE_HPP__
