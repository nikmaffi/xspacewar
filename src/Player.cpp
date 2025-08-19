#include <Player.hpp>

Player::Player(const Vector2 &pos, const Texture &texture, const Texture &laserTex, float angle, float scale, const Sound &laserSound, const Sound &explosionSound) :
sprint(false),
projectilesCount(PLAYER_MAX_PROJECTILES),
fuel(PLAYER_MAX_FUEL),
capturedByGravityWell(false),
dead(false),
pos(pos),
startPos(pos),
angle(angle),
startAngle(angle),
vel((Vector2){0.f, 0.f}),
texture(texture),
sprite((Rectangle){0.f, texture.height / 2.f, (float)texture.width, texture.height / 2.f}),
collisionBox(Circle(pos, sprite.height / 2.f)),
color(WHITE),
scale(scale),
laserTexture(laserTex),
laserSound(laserSound),
explosionTimer(EXPLOSION_WAIT_TIME + 1),
fireTimer(.0f),
hyperspaceTimer(PLAYER_HYPERSPACE_READY),
dist(-1.f, 1.f),
particles(EXPLOSION_LARGE_PARTICLES, EXPLOSION_COLOR),
explosionSound(explosionSound) {
}

const Circle &Player::getCircle(void) const {
	return collisionBox;
}

bool Player::isCollidingWith(const Circle &other) const {
    return collisionBox.overlaps(other);
}

bool Player::isExploding(void) const {
	return explosionTimer <= EXPLOSION_WAIT_TIME;
}

bool Player::isDead(void) const {
	return dead;
}

bool Player::isInHyperspace(void) const {
	return hyperspaceTimer > 0.f && hyperspaceTimer <= PLAYER_HYPERSPACE_TIME;
}

bool Player::isCapturedByGravityWell(void) const {
	return capturedByGravityWell;
}

void Player::reloadTextures(const Texture &playerTex, const Texture &laserTex) {
    texture = playerTex;
    laserTexture = laserTex;
}

void Player::reset(void) {
	// Resetting players atributes
	sprint = false;
	projectilesCount = __oneShotOneKill ? 1 : PLAYER_MAX_PROJECTILES;
	fuel = PLAYER_MAX_FUEL;
	capturedByGravityWell = false;
	dead = false;

	pos = startPos;
	angle = startAngle;
	vel = (Vector2){0.f, 0.f};

	collisionBox.center = startPos;

	projectiles.clear();

	explosionTimer = EXPLOSION_WAIT_TIME + 1;
	fireTimer = .0f;
	hyperspaceTimer = PLAYER_HYPERSPACE_READY;
}

void Player::rotate(float angle) {
	// Preventing player rotation if is exploding or is in hyperspace
	if(isExploding() || isInHyperspace()) {
		return;
	}

	// Updating the angle by adding the increment and wrapping it within the [0, 360) range
	this->angle = (float)std::fmod(this->angle + angle, 360.f);
}

void Player::move(float step) {
	// Preventing movement if out of fuel, exploding, or in hyperspace
	if(fuel <= 0 || isExploding() || isInHyperspace()) {
		return;
	}

	// Setting the sprite to the "sprinting" frame (top half of the texture)
	sprite = (Rectangle){0.f, 0.f, (float)texture.width, texture.height / 2.f};

	// Enabling sprint mode and consume fuel
	sprint = true;
	fuel -= 1 * __shipFuelLimit;

	// Moving the player in the direction of the current angle
	move((Vector2){std::cos(angle * DEG2RAD) * step, std::sin(angle * DEG2RAD) * step});
}

void Player::move(const Vector2 &acc) {
	// Updating velocity with acceleration
	vel.x += acc.x;
    vel.y += acc.y;

	// Updating position using velocity, scaled by frame time and a speed multiplier
	pos.x += vel.x * GetFrameTime() * DT_MULTIPLIER;
    pos.y += vel.y * GetFrameTime() * DT_MULTIPLIER;
}

void Player::hyperspace(void) {
	// Preventing hyperspace if is in hyperspace, is exploding or still in cooldown
	if(isExploding() || isInHyperspace() || hyperspaceTimer < PLAYER_HYPERSPACE_READY) {
		return;
	}

	// Activating hyperspace mode
	hyperspaceTimer = PLAYER_HYPERSPACE_TIME;
}

void Player::shoot(void) {
	// Preventing shooting if still in cooldown, out of projectiles, exploding, or in hyperspace
	if(fireTimer > 0 || projectilesCount <= 0 || isExploding() || isInHyperspace()) {
		return;
	}

	// Resetting the fire cooldown timer
	fireTimer = PLAYER_FIRE_TIME;
	// Consuming one projectile based on ship's projectile limit
	projectilesCount -= 1 * __shipProjectilesLimit;

	// Computing the spawn distance from the player's center
	float dist = sprite.height / 2;

	// Creating and adding a new laser projectile in the direction of the current player angle
	projectiles.push_back(
		Laser(
            (Vector2){pos.x + dist * std::cos(angle * DEG2RAD), pos.y + dist * std::sin(angle * DEG2RAD)},
            laserTexture,
            angle,
            LASER_TEX_SCALE,
			color.a
        )
	);

	// Playing laser firing sound
	PlaySound(laserSound);

	return;
}

void Player::update(Player *others, size_t self, const Anomaly &anomaly) {
	// Checking projectiles state
	for(size_t i = 0; i < projectiles.size(); i++) {
		if(projectiles[i].explosionTimer <= 0) {
			// Laser has exploded
			projectiles.erase(projectiles.begin() + i);
			continue;
		} else if(projectiles[i].explosionTimer == EXPLOSION_WAIT_TIME) {
			// Laser energy is finished
            projectiles[i].particles.init(projectiles[i].pos, projectiles[i].color.a);
		} else if(projectiles[i].explosionTimer >= EXPLOSION_SMALL_TIME && projectiles[i].isExploding()) {
			// Laser is exploding
			projectiles[i].particles.expandBy(EXPLOSION_PARTICLE_VELOCITY);
		}

		// Updating laser explosion timer if is exploding
		if(projectiles[i].isExploding()) {
			projectiles[i].explosionTimer -= GetFrameTime();
			continue;
		}

		// Moving the laser
		projectiles[i].move();
		// If the anomaly is a black hole, apply its gravitational pull to the projectile
		if(__blackHoleAsAnomaly) {
			projectiles[i].move(anomaly.attract(projectiles[i].pos));
		} 

		if(projectiles[i].isCollapsing()) {
			// Setting explosion timer in order to start explosion animation
			projectiles[i].explosionTimer = EXPLOSION_WAIT_TIME;
		} else if(projectiles[i].isCollidingWith(anomaly.getCircle())) {
			// Laser has fallen into the anomaly
			projectiles.erase(projectiles.begin() + i);
		} else {
			// Checking laser collision with other players, except the current one
			for(size_t k = 0; k < __numPlayers; k++) {\
				// Checking if k indexes the current player
				if(k == self || others[k].dead) {
					continue;
				}

				// Checking player k collision
				if(projectiles[i].isCollidingWith(others[k].getCircle()) && !others[k].isExploding() && !others[k].isInHyperspace()) {
					// Removing the laser
					projectiles.erase(projectiles.begin() + i);
					// Setting hit player explosion timer in order to start explosion animation
					others[k].explosionTimer = EXPLOSION_WAIT_TIME;

					// Playing explosion sound
					PlaySound(explosionSound);

					break;
				} else {
					// Checking laser collision with other player k lasers
					for(size_t j = 0; j < others[k].projectiles.size(); j++) {
						// Checking collision
						if(projectiles[i].isCollidingWith(others[k].projectiles[j].getCircle()) && !others[k].projectiles[j].isExploding()) {
							// Setting both laser explosion timers in order to start explosion animation
							projectiles[i].explosionTimer = EXPLOSION_WAIT_TIME;
							others[k].projectiles[j].explosionTimer = EXPLOSION_WAIT_TIME;
							break;
						}
					}
				}
			}
		}
	}

	if(dead) {
		return;
	}

	// Updating player explosion animation
	if(explosionTimer <= 0) {
		dead = true;
	} else if(explosionTimer == EXPLOSION_WAIT_TIME && !capturedByGravityWell) {
        particles.init(pos, color.a);
	} else if(explosionTimer >= EXPLOSION_LARGE_TIME && isExploding() && !capturedByGravityWell) {
		particles.expandBy(EXPLOSION_PARTICLE_VELOCITY);
	}

	// Updating explosion timer
	if(isExploding()) {
		explosionTimer -= GetFrameTime();
		return;
	}

	// Updating shooting cooldown
	if(fireTimer > 0.f) {
		fireTimer -= GetFrameTime();
	}

	// Handling hyperspace state
	if(isInHyperspace()) {
		// Countdown hyperspace duration
		hyperspaceTimer -= GetFrameTime();
		return;
	} else if(hyperspaceTimer <= 0.f) {
		// Teleport to a random location within the screen bounds
		float diff = VT_MONITOR_RADIUS * std::sin(PI / 4);

		pos.x = WINDOW_CENTER_X + dist(rd) * diff;
		pos.y = WINDOW_CENTER_Y + dist(rd) * diff;

		// Resetting velocity after teleportation
		vel.x = vel.y = 0.f;

		// Starting hyperspace cooldown
		hyperspaceTimer = PLAYER_HYPERSPACE_RELOADING;
	} else if(hyperspaceTimer >= PLAYER_HYPERSPACE_RELOADING && hyperspaceTimer < PLAYER_HYPERSPACE_READY) {
		// Recharging hyperspace timer until it's ready again
		hyperspaceTimer += GetFrameTime();
	}

	// Computing the distance from the player to the center of the screen
	float dist = (float)std::sqrt(std::pow(pos.x - WINDOW_CENTER_X, 2) + std::pow(pos.y - WINDOW_CENTER_Y, 2));

	// Checking if the player is outside the circular screen area
	if(dist > VT_MONITOR_RADIUS) {
		// Reflecting the player position across the center of the screen
		pos = (Vector2){WINDOW_CENTER_X - (pos.x - WINDOW_CENTER_X), WINDOW_CENTER_Y - (pos.y - WINDOW_CENTER_Y)};
	}

	// Check collision with the anomaly
	if(isCollidingWith(anomaly.getCircle())) {
		capturedByGravityWell = true;
		explosionTimer = EXPLOSION_WAIT_TIME;
		return;
	}

	// Check collision with other players
	for(size_t i = 0; i < __numPlayers; i++) {
		// Checking if i indexes the current player or the player i is dead
		if(i == self || others[i].dead) {
			continue;
		}

		if(isCollidingWith(others[i].getCircle()) && !others[i].isExploding() && !others[i].isInHyperspace()) {
			// Trigger explosion for both players
			explosionTimer = EXPLOSION_WAIT_TIME;
			others[i].explosionTimer = EXPLOSION_WAIT_TIME;

			// Playing explosion sound
			PlaySound(explosionSound);

			return;
		}
	}

	// Apply gravitational pull from anomaly
	move(anomaly.attract(pos));

	// Update collision box position
	collisionBox.center = pos;
}

void Player::draw(void) {
	// Drawing all projectiles
	for(size_t i = 0; i < projectiles.size(); i++) {
		projectiles[i].draw();
	}

	if(dead) {
		return;
	}

	// Drawing explosion particles, if player is exploding
	if(explosionTimer >= EXPLOSION_LARGE_TIME && isExploding() && !capturedByGravityWell) {
		particles.draw();
	}

	if(isExploding()) {
		return;
	}

	if(isInHyperspace()) {
		return;
	}

	// Resetting the flickering timer
	if(__flickeringTimer >= FLICKERING_INTERVAL) {
		// Simulating flickering effect
		// Adjusting texture alpha value based on previous one
		if(color.a != 255) {
			color.a = 255;
		} else {
			color.a = __flickeringEffectValue;
		}
	}

	// Restoring the sprite to the "non-sprinting" frame (bottom half of the texture)
	if(sprint) {
		sprint = false;
	} else {
        sprite = (Rectangle){.0f, texture.height / 2.f, (float)texture.width, texture.height / 2.f};
	}

	// Drawing rescaled sprite
    DrawTexturePro(
        texture,
        sprite,
        (Rectangle){pos.x, pos.y, sprite.width * scale, sprite.height * scale},
        (Vector2){sprite.width * scale / 2.f, sprite.height * scale / 2.f},
        angle,
        color
    );
}
