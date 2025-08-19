#include <Anomaly.hpp>

Anomaly::Anomaly(const Vector2 &pos, const Texture &texture, float scale):
mass(ANOMALY_MASS),
pos(pos),
angle(.0f),
texture(texture),
sprite((Rectangle){.0f, .0f, (float)texture.width, texture.height / 2.f}),
collisionBox(Circle(pos, ANOMALY_COLLISION_RADIUS)),
color(WHITE),
scale(scale),
flickeringTimer(.0f),
dist(0.f, 359.f) {
}

const Vector2 &Anomaly::getPosition(void) const {
	return pos;
}

const Circle &Anomaly::getCircle(void) const {
	return collisionBox;
}

void Anomaly::changeAnomaly(void) {
	// Changing anomaly sprite based on settings
	if(__blackHoleAsAnomaly) {
        sprite = (Rectangle){.0f, texture.height / 2.f, (float)texture.width, texture.height / 2.f};
	} else {
        sprite = (Rectangle){.0f, .0f, (float)texture.width, texture.height / 2.f};
	}
}

Vector2 Anomaly::attract(const Vector2 &objPos) const {
	// Orbit equation: \vec{a} = (G * m / |d|^3) * \vec{d}
	// EPSILON is used to avoid divisions by zero
	Vector2 dist = (Vector2){pos.x - objPos.x, pos.y - objPos.y};
	float distMag = std::sqrt(dist.x * dist.x + dist.y * dist.y) * 1 / SCALE_FACTOR + EPSILON;
	float forceMag = G * this->mass / (distMag * distMag * distMag);

	// Computing the acceleration
	return (Vector2){
        (forceMag * dist.x) * GetFrameTime() * DT_MULTIPLIER,
        (forceMag * dist.y) * GetFrameTime() * DT_MULTIPLIER
    };
}

void Anomaly::draw(void) {
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

	// Simulating star brightness by randomizing the direction angle
	angle = dist(rd);

	// Drawing rescaled texture
    DrawTexturePro(
        texture,
        sprite,
        (Rectangle){pos.x, pos.y, sprite.width * scale, sprite.height * scale},
        (Vector2){sprite.width * scale / 2.f, sprite.height * scale / 2.f},
        angle,
        color
    );
}
