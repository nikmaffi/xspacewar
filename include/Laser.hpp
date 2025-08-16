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
	friend class Player;
	friend class Phosphorus;
	friend struct GhostEntity;
	friend struct GhostParticles;

private:
	// Physics attributes
	Vector2 pos;
	float angle;
	Vector2 vel;
	float dispersion;
	
	// Game object attributes
	Texture texture;
	Circle collisionBox;
    Color color;
    float scale;

	// Timers
	float flickeringTimer;
	float explosionTimer;

	// Explosion attributes
	Particles particles;
public:
	Laser(const Vector2 &pos, const Texture &texture, float angle, float scale);
	Laser(const Laser &laser);
	Laser &operator=(const Laser &laser);
	bool isExploding(void) const;
	const Circle &getCircle(void) const;
	bool isCollidingWith(const Circle &other) const;
	bool isCollapsing(void)const;
	void move(void);
	void move(const Vector2 &acc);
	void draw(void);
};

#endif //__LASER_HPP__
