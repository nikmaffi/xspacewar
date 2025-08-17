#include <Laser.hpp>

Laser::Laser(const Vector2 &pos, const Texture &texture, float angle, float scale):
pos(pos),
angle(angle),
vel((Vector2){LASER_SPEED * std::cos(angle * DEG2RAD), LASER_SPEED * std::sin(angle * DEG2RAD)}),
dispersion(LASER_ENERGY),
texture(texture),
collisionBox(Circle(pos, texture.width / 2.f)),
color(WHITE),
scale(scale),
flickeringTimer(.0f),
explosionTimer(EXPLOSION_WAIT_TIME + 1),
particles(EXPLOSION_SMALL_PARTICLES, EXPLOSION_COLOR) {
}

Laser::Laser(const Laser &laser):
collisionBox(Circle(laser.pos, laser.texture.width / 2.f)),
particles(EXPLOSION_SMALL_PARTICLES, EXPLOSION_COLOR) {
	*this = laser;
}

Laser& Laser::operator=(const Laser &laser) {
	// Copying attributes
	pos = laser.pos;
	angle = laser.angle;
	vel = laser.vel;
	dispersion = laser.dispersion;

	texture = laser.texture;
    color = laser.color;
	scale = laser.scale;

	flickeringTimer = laser.flickeringTimer;
	explosionTimer = laser.explosionTimer;

	// Operator = must return a reference to lvalue
	return *this;
}

bool Laser::isExploding(void) const {
	return explosionTimer <= EXPLOSION_WAIT_TIME;
}

const Circle &Laser::getCircle(void) const {
	return collisionBox;
}

bool Laser::isCollidingWith(const Circle &other) const {
    return collisionBox.overlaps(other);
}

bool Laser::isCollapsing(void) const {
	return dispersion <= 0.f;
}

void Laser::move(void) {
	// Decreasing laser energy
	dispersion -= LASER_DISPERSION_RATE;

	// Computing the distance from the laser to the center of the screen
	float dist = (float)std::sqrt(std::pow(pos.x - WINDOW_CENTER_X, 2) + std::pow(pos.y - WINDOW_CENTER_Y, 2));

	// Checking if the laser is outside the circular screen area
	if(dist > VT_MONITOR_RADIUS) {
		// Reflecting the laser position across the center of the screen
		pos = (Vector2){WINDOW_CENTER_X - (pos.x - WINDOW_CENTER_X), WINDOW_CENTER_Y - (pos.y - WINDOW_CENTER_Y)};
	}

	// Updating laser position by laser speed
    pos.x += vel.x;
    pos.y += vel.y;

	// Updating collision box position
	collisionBox.center = pos;
}

void Laser::move(const Vector2 &acc) {
    // Adding acceleration to velocity
    vel.x += acc.x;
    vel.y += acc.y;

    // Updating position based on acceleration
    pos.x += acc.x;
    pos.y += acc.y;

	// Updating collision box position
	collisionBox.center = pos;
}

void Laser::draw(void) {
	// Drawing the laser explosion
	if(explosionTimer >= EXPLOSION_SMALL_TIME && isExploding()) {
		particles.draw();
	}

	if(isExploding()) {
		return;
	}

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

	// Drawing rescaled texture
    DrawTexturePro(
        texture,
        (Rectangle){.0f, .0f, (float)texture.width, (float)texture.height},
        (Rectangle){pos.x, pos.y, texture.width * scale, texture.height * scale},
        (Vector2){texture.width * scale / 2.f, texture.height * scale / 2.f},
        angle,
        color
    );
}
