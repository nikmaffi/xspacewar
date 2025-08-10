#include <Laser.hpp>

Laser::Laser(const Vector2 &pos, const Texture &texture, float angle, float scale):
pos(pos),
angle(angle),
vel((Vector2){LASER_SPEED * std::cos(angle * DEG_TO_RAD), LASER_SPEED * std::sin(angle * DEG_TO_RAD)}),
mass(7.f),
texture(texture),
scale(scale),
flickeringTimer(.0f),
color(WHITE),
explosionTimer(WAIT_TIME + 1),
particles(SMALL_EXPLOSION_PARTICLES, EXPLOSION_COLOR),
dispersion(LASER_ENERGY) {
}

Laser::Laser(const Laser &laser): particles(SMALL_EXPLOSION_PARTICLES, EXPLOSION_COLOR) {
	*this = laser;
}

Laser& Laser::operator=(const Laser &laser) {
	pos = laser.pos;
	angle = laser.angle;
	vel = laser.vel;
	mass = laser.mass;
	texture = laser.texture;
	scale = laser.scale;
	flickeringTimer = laser.flickeringTimer;
    color = laser.color;
	explosionTimer = laser.explosionTimer;
	dispersion = laser.dispersion;

	return *this;
}

bool Laser::isExploding(void) const {
	return explosionTimer <= WAIT_TIME;
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

	if(dist > MONITOR_RADIUS) {
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
	if(explosionTimer >= SMALL_EXPLOSION_TIME && isExploding()) {
		particles.draw();
	}

	if(isExploding()) {
		return;
	}

	flickeringTimer += GetFrameTime() * flickeringMonitorEffect;

	if(flickeringTimer >= FLICKER_FRAME_INTERVAL) {
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
        (Vector2){texture.width / 2.f, texture.height / 2.f},
        angle,
        color
    );
}