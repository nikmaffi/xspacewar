#ifndef __PLAYER_HPP__
#define __PLAYER_HPP__

#include <xspacewar.hpp>
#include <Laser.hpp>
#include <Anomaly.hpp>
#include <Circle.hpp>
#include <Particles.hpp>

class Phosphorus;
class GhostEntity;
class GhostParticles;

class Player {
private:
	Vector2 pos;
	Vector2 startPos;
	float angle;
	float startAngle;
	Vector2 vel;
	float mass;

	Texture texture;
	Rectangle sprite;
	Color color;
	float scale;

	std::vector<Laser> projectiles;

	std::random_device rd;
	std::uniform_real_distribution<float> dist;

	float flickeringTimer;

	float explosionTimer;
	Particles particles;

	Texture laserTexture;

	bool sprint;
	float fireTimer;
	float hyperspaceTimer;
	int projectilesCount;
	int fuel;

	bool capturedByGravityWell;
public:
	Player(const Vector2 &pos, const char *textureRes, const char *laserTextureRes, float angle = 0.f, float scale = 1.f);
	~Player();
	Circle getCircle(void) const;
	bool isCollidingWith(const Circle &other) const;
	bool isExploding(void) const;
	bool isInHyperspace(void) const;
	void reloadTextures(const char *playerTextureRes, const char *laserTextureRes);
	void reset(void);
	void rotate(float angle);
	void move(float step);
	void move(const Vector2 &acc);
	void hyperspace(void);
	void shoot(void);
	void update(Player &other, const Anomaly &anomaly);
	void draw(void);

	friend class Phosphorus;
	friend class GhostEntity;
	friend class GhostParticles;
};

#endif //__PLAYER_HPP__