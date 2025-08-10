#ifndef __ANOMALY_HPP__
#define __ANOMALY_HPP__

#include <xspacewar.hpp>
#include <Circle.hpp>

class GhostEntity;

class Anomaly {
private:
	Vector2 pos;
	float mass;

	float angle;

	Texture texture;
    Rectangle sprite;
    Color color;
    float scale;

	float flickeringTimer;

	std::random_device rd;
	std::uniform_real_distribution<float> dist;
public:
	Anomaly(const Vector2 &pos, const char *textureRes, float scale);
	~Anomaly();
	const Vector2 &getPosition(void) const;
	Circle getCircle(void) const;
	void changeAnomaly(void);
	Vector2 attract(float mass, const Vector2 &pos) const;
	void draw(void);

	friend class GhostEntity;
};

#endif //__ANOMALY_HPP__