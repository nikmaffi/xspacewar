#include <Player.hpp>

Player::Player(const Vector2 &pos, const char *textureRes, const char *laserTextureRes, float angle, float scale) :
pos(pos),
startPos(pos),
angle(angle),
startAngle(angle),
vel((Vector2){0.f, 0.f}),
mass(5.f),
texture(LoadTexture(textureRes)),
sprite((Rectangle){0.f, texture.height / 2.f, (float)texture.width, texture.height / 2.f}),
color(WHITE),
scale(scale),
dist(-1.f, 1.f),
flickeringTimer(.0f),
explosionTimer(WAIT_TIME + 1),
particles(LARGE_EXPLOSION_PARTICLES, EXPLOSION_COLOR),
laserTexture(LoadTexture(laserTextureRes)),
sprint(false),
fireTimer(.0f),
hyperspaceTimer(HYPERSPACE_READY),
projectilesCount(SHIP_MAX_PROJECTILES),
fuel(SHIP_MAX_FUEL),
capturedByGravityWell(false) {
}

Player::~Player() {
	UnloadTexture(texture);
	UnloadTexture(laserTexture);
}

Circle Player::getCircle() const {
	return Circle(pos, sprite.height / 2.f);
}

bool Player::isCollidingWith(const Circle &other) const {
    return getCircle().overlaps(other);
}

bool Player::isExploding() const {
	return explosionTimer <= WAIT_TIME;
}

bool Player::isInHyperspace() const {
	return hyperspaceTimer > 0.f && hyperspaceTimer <= HYPERSPACE_TIME;
}

void Player::reloadTextures(const char *playerTextureRes, const char *laserTextureRes) {
	UnloadTexture(texture);
    texture = LoadTexture(playerTextureRes);

	UnloadTexture(laserTexture);
    laserTexture = LoadTexture(laserTextureRes);
}

void Player::reset(void) {
	pos = startPos;
	angle = startAngle;
	vel = (Vector2){0.f, 0.f};
	projectiles.clear();
	flickeringTimer = .0f;
	explosionTimer = WAIT_TIME + 1;
	sprint = false;
	fireTimer = .0f;
	hyperspaceTimer = HYPERSPACE_READY;
	projectilesCount = oneShotOneKill ? 1 : SHIP_MAX_PROJECTILES;
	fuel = SHIP_MAX_FUEL;
	capturedByGravityWell = false;
}

void Player::rotate(float angle) {
	if(isExploding() || isInHyperspace()) {
		return;
	}

	this->angle = (float)std::fmod(this->angle + angle, 360.f);
}

void Player::move(float step) {
	if(fuel <= 0 || isExploding() || isInHyperspace()) {
		return;
	}

	sprite = (Rectangle){0.f, 0.f, (float)texture.width, texture.height / 2.f};

	sprint = true;
	fuel -= 1 * shipFuelLimit;

	move((Vector2){std::cos(angle * DEG_TO_RAD) * step, std::sin(angle * DEG_TO_RAD) * step});
}

void Player::move(const Vector2 &acc) {
	vel.x += acc.x;
    vel.y += acc.y;

	pos.x += vel.x * GetFrameTime() * MULTIPLIER;
    pos.y += vel.y * GetFrameTime() * MULTIPLIER;
}

void Player::hyperspace(void) {
	if(isExploding() || isInHyperspace() || hyperspaceTimer < HYPERSPACE_READY) {
		return;
	}

	hyperspaceTimer = HYPERSPACE_TIME;
}

void Player::shoot() {
	if(fireTimer > 0 || projectilesCount <= 0 || isExploding() || isInHyperspace()) {
		return;
	}

	fireTimer = FIRE_TIME;
	projectilesCount -= 1 * shipProjectilesLimit;

	float dist = sprite.height / 2;

	projectiles.push_back(
		Laser(
            (Vector2){pos.x + dist * std::cos(angle * DEG_TO_RAD), pos.y + dist * std::sin(angle * DEG_TO_RAD)},
            laserTexture,
            angle,
            .5f * WINDOW_WIDTH / WSCALE
        )
	);
}

void Player::update(Player &other, const Anomaly &anomaly) {
	for(size_t i = 0; i < projectiles.size(); i++) {
		if(projectiles[i].explosionTimer <= 0) {
			projectiles.erase(projectiles.begin() + i);
			continue;
		} else if(projectiles[i].explosionTimer == WAIT_TIME) {
            projectiles[i].particles.init(projectiles[i].pos);
		} else if(projectiles[i].explosionTimer >= SMALL_EXPLOSION_TIME && projectiles[i].isExploding()) {
			projectiles[i].particles.expandBy(PARTICLE_VELOCITY);
		}

		if(projectiles[i].isExploding()) {
			projectiles[i].explosionTimer -= GetFrameTime();
			continue;
		}

		projectiles[i].move();
		if(blackHoleAsAnomaly) {
			projectiles[i].move(anomaly.attract(projectiles[i].mass, projectiles[i].pos));
		}

		if(projectiles[i].isCollapsing()) {
			projectiles[i].explosionTimer = WAIT_TIME;
		} else if(projectiles[i].isCollidingWith(other.getCircle()) && !other.isExploding() && !other.isInHyperspace()) {
			projectiles.erase(projectiles.begin() + i);
			other.explosionTimer = WAIT_TIME;
		} else if(projectiles[i].isCollidingWith(anomaly.getCircle())) {
			projectiles.erase(projectiles.begin() + i);
		} else {
			for(size_t j = 0; j < other.projectiles.size(); j++) {
				if(projectiles[i].isCollidingWith(other.projectiles[j].getCircle())) {
					projectiles[i].explosionTimer = WAIT_TIME;
					other.projectiles[j].explosionTimer = WAIT_TIME;
					break;
				}
			}
		}
	}

	if(explosionTimer <= 0) {
		reset();
		other.reset();
	} else if(explosionTimer == WAIT_TIME && !capturedByGravityWell) {
        particles.init(pos);
	} else if(explosionTimer >= LARGE_EXPLOSION_TIME && isExploding() && !capturedByGravityWell) {
		particles.expandBy(PARTICLE_VELOCITY);
	}

	if(isExploding()) {
		explosionTimer -= GetFrameTime();
		return;
	}

	if(fireTimer > 0.f) {
		fireTimer -= GetFrameTime();
	}

	if(isInHyperspace()) {
		hyperspaceTimer -= GetFrameTime();
		return;
	} else if(hyperspaceTimer <= 0.f) {
		float diff = MONITOR_RADIUS * std::sin(PI / 4);

		pos.x = WINDOW_CENTER_X + dist(rd) * diff;
		pos.y = WINDOW_CENTER_Y + dist(rd) * diff;

		vel.x = vel.y = 0.f;

		hyperspaceTimer = HYPERSPACE_RELOADING;
	} else if(hyperspaceTimer >= HYPERSPACE_RELOADING && hyperspaceTimer < HYPERSPACE_READY) {
		hyperspaceTimer += GetFrameTime();
	}

	float dist = (float)std::sqrt(std::pow(pos.x - WINDOW_CENTER_X, 2) + std::pow(pos.y - WINDOW_CENTER_Y, 2));

	if(dist > MONITOR_RADIUS) {
		pos = (Vector2){WINDOW_CENTER_X - (pos.x - WINDOW_CENTER_X), WINDOW_CENTER_Y - (pos.y - WINDOW_CENTER_Y)};
	}

	if(isCollidingWith(anomaly.getCircle())) {
		capturedByGravityWell = true;
		explosionTimer = WAIT_TIME;
		return;
	}

	if(isCollidingWith(other.getCircle()) && !other.isExploding() && !other.isInHyperspace()) {
		explosionTimer = WAIT_TIME;
		other.explosionTimer = WAIT_TIME;
		return;
	}

	move(anomaly.attract(mass, pos));
}

void Player::draw(void) {
	for(size_t i = 0; i < projectiles.size(); i++) {
		projectiles[i].draw();
	}

	if(explosionTimer >= LARGE_EXPLOSION_TIME && isExploding() && !capturedByGravityWell) {
		particles.draw();
	}

	if(isExploding()) {
		return;
	}

	if(isInHyperspace()) {
		return;
	}

	flickeringTimer += GetFrameTime() * flickeringMonitorEffect;

	if(flickeringTimer >= FLICKER_FRAME_INTERVAL) {
		flickeringTimer = .0f;

		if(color.a == 185) {
			color.a = 255;
		} else {
			color.a = 185;
		}
	}

	if(sprint) {
		sprint = false;
	} else {
        sprite = (Rectangle){.0f, texture.height / 2.f, (float)texture.width, texture.height / 2.f};
	}

    DrawTexturePro(
        texture,
        sprite,
        (Rectangle){pos.x, pos.y, sprite.width * scale, sprite.height * scale},
        (Vector2){sprite.width / 2.f, sprite.height / 2.f},
        angle,
        color
    );
}