#ifndef __PARTICLES_HPP__
#define __PARTICLES_HPP__

#include <xspacewar.hpp>

struct GhostParticles;

class Particles {
    friend struct GhostParticles;

private:
    Vector2 *pos;
    Vector2 *dir;
    unsigned int size;

    // Explosion color
    Color color;

    // Random number generator
    std::random_device rd;
	std::uniform_real_distribution<float> dist;
public:
    Particles(unsigned int size, const Color &color);
    ~Particles();
    Particles &operator=(const Particles &p);
    void init(const Vector2 &pos, unsigned char alpha);
    void expandBy(float factor);
    void draw(void);
};

#endif //__PARTICLES_HPP__
