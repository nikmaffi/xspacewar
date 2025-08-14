#include <Particles.hpp>

Particles::Particles(unsigned int size, const Color &color):
pos(new Vector2[size]),
dir(new Vector2[size]),
size(size),
flickeringTimer(.0f),
color(color),
dist(-1.f, 1.f) {
}

Particles::~Particles() {
    delete[] pos;
    delete[] dir;
}

Particles &Particles::operator=(const Particles &p) {
    pos = p.pos;
    dir = p.dir;
    size = p.size;
    flickeringTimer = p.flickeringTimer;
    color = p.color;

    return *this;
}

void Particles::init(const Vector2 &pos) {
    for(unsigned int i = 0; i < size; i++) {
        this->pos[i].x = pos.x;
        this->pos[i].y = pos.y;
        dir[i].x = dist(rd);
        dir[i].y = dist(rd);
    }
}

void Particles::expandBy(float factor) {
    for(unsigned int i = 0; i < size; i++) {
        pos[i].x += dir[i].x * factor;
        pos[i].y += dir[i].y * factor;
    }
}

void Particles::draw(void) {
    if(flickeringTimer >= FLICKERING_INTERVAL) {
        flickeringTimer = .0f;

        if(color.a == FLICKERING_ALPHA) {
            color.a = 255;
        } else {
            color.a = FLICKERING_ALPHA;
        }
    }

    for(unsigned int i = 0; i < size; i++) {
        DrawPixel(pos[i].x, pos[i].y, color);
    }
}
