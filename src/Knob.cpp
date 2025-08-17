#include <Knob.hpp>

Knob::Knob(const Vector2 &pos, const Texture &texture, float scale):
pos(pos),
angle(.0f),
texture(texture),
scale(scale) {
}

void Knob::setAngle(float angle) {
    this->angle = angle;
}

void Knob::update(const Vector2 &mousePos, void (*updateValue)(float)) {
    // Calculating the distance between the knob center and mouse position
    float dist = std::sqrt(std::pow(pos.x - mousePos.x, 2) + std::pow(pos.y - mousePos.y, 2));

    // Checking if the mouse position is over the knob
    if(dist > texture.width / 2.f * scale) {
        return;
    }

    // Limiting knob angle value between [0, 180] degrees
    if(mousePos.y <= pos.y) {
        // Calculating the angle value based on mouse position
        angle = RAD2DEG * std::acos((pos.x - mousePos.x) / dist);
        updateValue(angle);
    }
}

void Knob::draw(void) {
    // Drawing rescaled texture
    DrawTexturePro(
        texture,
        {.0f, .0f, (float)texture.width, (float)texture.height},
        {pos.x, pos.y, texture.width * scale, texture.height * scale},
        {texture.width * scale / 2.f, texture.height * scale / 2.f},
        angle,
        WHITE
    );
}
