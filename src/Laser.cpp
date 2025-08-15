#include <Laser.hpp>

Laser::Laser(const Vector2 &pos, const Texture &texture, float angle, float scale):
pos(pos),
angle(angle),
vel((Vector2){LASER_SPEED * std::cos(angle * DEG2RAD), LASER_SPEED * std::sin(angle * DEG2RAD)}),
texture(texture),
scale(scale),
flickeringTimer(.0f),
color(WHITE),
explosionTimer(EXPLOSION_WAIT_TIME + 1),
particles(EXPLOSION_SMALL_PARTICLES, EXPLOSION_COLOR),
dispersion(LASER_ENERGY) {
}

Laser::Laser(const Laser &laser): particles(EXPLOSION_SMALL_PARTICLES, EXPLOSION_COLOR) {
	*this = laser;
}

Laser& Laser::operator=(const Laser &laser) {
	pos = laser.pos;
	angle = laser.angle;
	vel = laser.vel;
	texture = laser.texture;
	scale = laser.scale;
	flickeringTimer = laser.flickeringTimer;
    color = laser.color;
	explosionTimer = laser.explosionTimer;
	dispersion = laser.dispersion;

	return *this;
}

bool Laser::isExploding(void) const {
	return explosionTimer <= EXPLOSION_WAIT_TIME;
}

Circle Laser::getCircle(void) const {
	return Circle(pos, texture.height / 2);
}

bool Laser::isCollidingWith(const Circle &other) const {
    return getCircle().overlaps(other);
}

bool Laser::isCollapsing(void) const {
	return dispersion <= 0.f;
}

void Laser::move(void) {
	dispersion -= LASER_DISPERSION_RATE;

	float dist = (float)std::sqrt(std::pow(pos.x - WINDOW_CENTER_X, 2) + std::pow(pos.y - WINDOW_CENTER_Y, 2));

	if(dist > VT_MONITOR_RADIUS) {
		pos = (Vector2){WINDOW_CENTER_X - (pos.x - WINDOW_CENTER_X), WINDOW_CENTER_Y - (pos.y - WINDOW_CENTER_Y)};
	}

    pos.x += vel.x;
    pos.y += vel.y;
}

void Laser::move(const Vector2 &acc) {
	vel.x += acc.x;
    vel.y += acc.y;

	pos.x += acc.x;
	pos.y += acc.y;
}

void Laser::draw(void) {
	if(explosionTimer >= EXPLOSION_SMALL_TIME && isExploding()) {
		particles.draw();
	}

	if(isExploding()) {
		return;
	}

	flickeringTimer += GetFrameTime() * flickeringMonitorEffect;

	if(flickeringTimer >= FLICKERING_INTERVAL) {
		flickeringTimer = .0f;

		if(color.a == FLICKERING_ALPHA) {
			color.a = 255;
		} else {
			color.a = FLICKERING_ALPHA;
		}
	}

    DrawTexturePro(
        texture,
        (Rectangle){.0f, .0f, (float)texture.width, (float)texture.height},
        (Rectangle){pos.x, pos.y, texture.width * scale, texture.height * scale},
        (Vector2){texture.width * scale / 2.f, texture.height * scale / 2.f},
        angle,
        color
    );
}