#ifndef __LASER_HPP__
#define __LASER_HPP__

#include <xspacewar.hpp>
#include <Particles.hpp>
#include <Circle.hpp>

class Player;
class Phosphorus;
struct GhostEntity;
struct GhostParticles;

class Laser {
private:
	Vector2 pos;
	float angle;
	Vector2 vel;
	float mass;

	Texture texture;
    float scale;

	float flickeringTimer;
    Color color;

	float explosionTimer;
	Particles particles;

	float dispersion;
public:
	Laser(const Vector2 &pos, const Texture &texture, float angle, float scale);
	Laser(const Laser &laser);
	Laser &operator=(const Laser &laser);
	bool isExploding(void) const;
	Circle getCircle(void) const;
	bool isCollidingWith(const Circle &other) const;
	bool isCollapsing(void)const;
	void move(void);
	void move(const Vector2 &acc);
	void draw(void);

	friend class Player;
	friend class Phosphorus;
	friend struct GhostEntity;
	friend struct GhostParticles;
};

#endif //__LASER_HPP__
