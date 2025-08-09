#include <Circle.hpp>

Circle::Circle(const Vector2 &center, float radius): center(center), radius(radius) {
}

bool Circle::overlaps(const Circle &other) {
    float dist = (float)std::sqrt(std::pow(center.x - other.center.x, 2) + std::pow(center.y - other.center.y, 2));

    return dist <= radius + other.radius;
}