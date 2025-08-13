#ifndef __PARTICLES_HPP__
#define __PARTICLES_HPP__

#include <xspacewar.hpp>

struct GhostParticles;

class Particles {
private:
    Vector2 *pos;
    Vector2 *dir;
    unsigned int size;

    float flickeringTimer;
    Color color;

    std::random_device rd;
	std::uniform_real_distribution<float> dist;
public:
    Particles(unsigned int size, const Color &color);
    ~Particles();
    Particles &operator=(const Particles &p);
    void init(const Vector2 &pos);
    void expandBy(float factor);
    void draw(void);

    friend struct GhostParticles;
};

#endif //__PARTICLES_HPP__
