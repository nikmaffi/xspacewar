#ifndef __ANOMALY_HPP__
#define __ANOMALY_HPP__

#include <xspacewar.hpp>
#include <Circle.hpp>

struct GhostEntity;

class Anomaly {
	friend struct GhostEntity;

private:
	// Physics attributes
	float mass;
	Vector2 pos;
	float angle;
	
	// Game object attributes
	Texture texture;
    Rectangle sprite;
	Circle collisionBox;
    Color color;
    float scale;

	// Random number generator
	std::random_device rd;
	std::uniform_real_distribution<float> dist;
public:
	Anomaly(const Vector2 &pos, const Texture &texture, float scale);
	const Vector2 &getPosition(void) const;
	const Circle &getCircle(void) const;
	void changeAnomaly(void);
	Vector2 attract(const Vector2 &objPos) const;
	void draw(void);
};

#endif //__ANOMALY_HPP__
