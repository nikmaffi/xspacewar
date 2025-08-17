#include <Particles.hpp>

Particles::Particles(unsigned int size, const Color &color):
pos(new Vector2[size]),
dir(new Vector2[size]),
size(size),
color(color),
flickeringTimer(.0f),
dist(-1.f, 1.f) {
}

Particles::~Particles() {
    // Deleting dynamic arrays
    delete[] pos;
    delete[] dir;
}

Particles &Particles::operator=(const Particles &p) {
    // Operator = used by std::vector<Particles>
    pos = p.pos;
    dir = p.dir;
    size = p.size;
    flickeringTimer = p.flickeringTimer;
    color = p.color;

    return *this;
}

void Particles::init(const Vector2 &pos) {
    // Initializing particles by provided position
    for(unsigned int i = 0; i < size; i++) {
        this->pos[i].x = pos.x;
        this->pos[i].y = pos.y;
        // Simulating explosion effect by computing random directions for all particles
        dir[i].x = dist(rd);
        dir[i].y = dist(rd);
    }
}

void Particles::expandBy(float factor) {
    // Updating particles position simulating explosion effect
    for(unsigned int i = 0; i < size; i++) {
        pos[i].x += dir[i].x * factor;
        pos[i].y += dir[i].y * factor;
    }
}

void Particles::draw(void) {
    // Updating flickering effect timer
    flickeringTimer += GetFrameTime();

    // Resetting the flickering timer
    if(flickeringTimer >= FLICKERING_INTERVAL) {
        flickeringTimer = .0f;

        // Simulating flickering effect
        // Adjusting texture alpha value based on previous one
        if(color.a != 255) {
            color.a = 255;
        } else {
            color.a = __flickeringEffectValue;
        }
    }

    // Drawing particles
    for(unsigned int i = 0; i < size; i++) {
        DrawPixel(pos[i].x, pos[i].y, color);
    }
}
