#ifndef __PLAYER_HPP__
#define __PLAYER_HPP__

#include <xspacewar.hpp>
#include <Laser.hpp>
#include <Anomaly.hpp>
#include <Circle.hpp>
#include <Particles.hpp>

class Phosphorus;
struct GhostEntity;
struct GhostParticles;

class Player {
	friend class Phosphorus;
	friend struct GhostEntity;
	friend struct GhostParticles;

private:
	// Misc attributes
	bool sprint;
	int projectilesCount;
	int fuel;
	bool capturedByGravityWell;
	bool dead;

	// Physics attributes
	Vector2 pos;
	Vector2 startPos;
	float angle;
	float startAngle;
	Vector2 vel;
	
	// Game object attributes
	Texture texture;
	Rectangle sprite;
	Circle collisionBox;
	Color color;
	float scale;

	// Lasers attributes
	std::vector<Laser> projectiles;
	Texture laserTexture;
	Sound laserSound;

	// Timers
	float explosionTimer;
	float fireTimer;
	float hyperspaceTimer;

	// Random number generator
	std::random_device rd;
	std::uniform_real_distribution<float> dist;

	// Explosion attributes
	Particles particles;
	Sound explosionSound;
public:
	Player(const Vector2 &pos, const Texture &texture, const Texture &laserTex, float angle, float scale, const Sound &laserSound, const Sound &explosionSound);
	const Circle &getCircle(void) const;
	bool isCollidingWith(const Circle &other) const;
	bool isExploding(void) const;
	bool isDead(void) const;
	bool isInHyperspace(void) const;
	bool isCapturedByGravityWell(void) const;
	void reloadTextures(const Texture &playerTex, const Texture &laserTex);
	void reset(void);
	void rotate(float angle);
	void move(float step);
	void move(const Vector2 &acc);
	void hyperspace(void);
	void shoot(void);
	void update(Player *others, size_t self, const Anomaly &anomaly);
	void draw(void);
};

#endif //__PLAYER_HPP__
