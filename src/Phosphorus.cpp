#include <Phosphorus.hpp>

GhostEntity::GhostEntity(const Player &player):
pos(player.pos),
angle(player.angle),
texture(player.texture),
sprite(player.sprite),
color(PHOSPHORUS_COLOR(255)),
scale(player.scale),
timer(255.f) {
}

GhostEntity::GhostEntity(const Laser &laser):
pos(laser.pos),
angle(laser.angle),
texture(laser.texture),
sprite((Rectangle){.0f, .0f, (float)laser.texture.width, (float)laser.texture.height}),
color(PHOSPHORUS_COLOR(255)),
scale(laser.scale),
timer(255.f) {
}

GhostEntity::GhostEntity(const Anomaly &anomaly):
pos(anomaly.pos),
angle(anomaly.angle),
texture(anomaly.texture),
sprite(anomaly.sprite),
color(PHOSPHORUS_COLOR(255)),
scale(anomaly.scale),
timer(100.f) {
}

GhostParticles::GhostParticles(const Particles &particles, size_t size):
pos(particles.pos, particles.pos + size),
color(PHOSPHORUS_COLOR(255)),
timer(255.f) {
}

GhostInterface::GhostInterface(const Interface &interface):
pos(interface.pos),
color(PHOSPHORUS_COLOR(255)),
text(interface.text),
font(interface.font),
fontSize(interface.fontSize),
timer(100.f) {
}

Phosphorus::Phosphorus(const Player *players, const Anomaly &anomaly, const Interface &interface):
players(players),
anomaly(anomaly),
interface(interface) {
}

void Phosphorus::updatePlayer(const Player &player, bool monitorRunning) {
    if(monitorRunning && !player.isExploding() && !player.capturedByGravityWell && !player.isDead()) {
        entitiesPhosphorus.push_back(GhostEntity(player));
    }

    for(size_t i = 0; i < entitiesPhosphorus.size(); i++) {
        entitiesPhosphorus[i].timer -= GetFrameTime() * 15.f;

        if(entitiesPhosphorus[i].timer <= .0f) {
            entitiesPhosphorus.erase(entitiesPhosphorus.begin() + i);
			continue;
        }

        entitiesPhosphorus[i].color.a = (unsigned char)entitiesPhosphorus[i].timer;
    }
}

void Phosphorus::updatePlayerExplosion(const Player &player, bool monitorRunning) {
    if(monitorRunning && player.explosionTimer >= LARGE_EXPLOSION_TIME && player.isExploding() &&
    !player.capturedByGravityWell && !player.isDead()) {
		playerExplosionPhosphorus.push_back(GhostParticles(player.particles, LARGE_EXPLOSION_PARTICLES));
	}

	for(size_t i = 0; i < playerExplosionPhosphorus.size(); i++) {
		playerExplosionPhosphorus[i].timer -= GetFrameTime() * MULTIPLIER;

		if(playerExplosionPhosphorus[i].timer <= 0) {
			playerExplosionPhosphorus.erase(playerExplosionPhosphorus.begin() + i);
			continue;
		}

		playerExplosionPhosphorus[i].color.a = (unsigned char)playerExplosionPhosphorus[i].timer;
	}
}

void Phosphorus::updatePlayerLaser(const Player &player, bool monitorRunning) {
    for(size_t i = playerLaserPhosphorus.size(); i < player.projectiles.size(); i++) {
        playerLaserPhosphorus.push_back(std::vector<GhostEntity>());
    }

    for(size_t i = 0; i < player.projectiles.size(); i++) {
        if(monitorRunning && !player.projectiles[i].isExploding()) {
			playerLaserPhosphorus[i].push_back(GhostEntity(player.projectiles[i]));
		}
	}

    for(size_t i = 0; i < playerLaserPhosphorus.size(); i++) {
        for(size_t j = 0; j < playerLaserPhosphorus[i].size(); j++) {
            playerLaserPhosphorus[i][j].timer -= GetFrameTime() * MULTIPLIER;

            if(playerLaserPhosphorus[i][j].timer <= .0f) {
                playerLaserPhosphorus[i].erase(playerLaserPhosphorus[i].begin() + j);
            } else {
                playerLaserPhosphorus[i][j].color.a = (unsigned char)playerLaserPhosphorus[i][j].timer;
            }
        }
    }

    for(size_t i = 0; i < playerLaserPhosphorus.size(); i++) {
        if(playerLaserPhosphorus[i].empty()) {
            playerLaserPhosphorus.erase(playerLaserPhosphorus.begin() + i);
        }
    }
}

void Phosphorus::updatePlayerLaserExplosion(const Player &player, bool monitorRunning) {
    for(size_t i = playerLaserExplosionPhosphorus.size(); i < player.projectiles.size(); i++) {
        playerLaserExplosionPhosphorus.push_back(std::vector<GhostParticles>());
    }

	for(size_t i = 0; i < player.projectiles.size(); i++) {
		if(monitorRunning && player.projectiles[i].explosionTimer >= SMALL_EXPLOSION_TIME &&
            player.projectiles[i].isExploding()) {
			playerLaserExplosionPhosphorus[i].push_back(
                GhostParticles(player.projectiles[i].particles, SMALL_EXPLOSION_PARTICLES)
            );
		}
	}

    for(size_t i = 0; i < playerLaserExplosionPhosphorus.size(); i++) {
        for(size_t j = 0; j < playerLaserExplosionPhosphorus[i].size(); j++) {
            playerLaserExplosionPhosphorus[i][j].timer -= GetFrameTime() * MULTIPLIER;

            if(playerLaserExplosionPhosphorus[i][j].timer <= .0f) {
                playerLaserExplosionPhosphorus[i].erase(playerLaserExplosionPhosphorus[i].begin() + j);
            } else {
                playerLaserExplosionPhosphorus[i][j].color.a = (unsigned char)playerLaserExplosionPhosphorus[i][j].timer;
            }
        }
    }

    for(size_t i = 0; i < playerLaserPhosphorus.size(); i++) {
        if(playerLaserPhosphorus[i].empty()) {
            playerLaserPhosphorus.erase(playerLaserPhosphorus.begin() + i);
        }
    }
}

void Phosphorus::update(bool monitorRunning) {
    // Anomaly
	if(monitorRunning) {
		entitiesPhosphorus.push_back(GhostEntity(anomaly));
	}

    // Interface
    if(!monitorRunning) {
		interfacePhosphorus.push_back(GhostInterface(interface));
	}

	for(size_t i = 0; i < interfacePhosphorus.size(); i++) {
		interfacePhosphorus[i].timer -= GetFrameTime() * MULTIPLIER;

		if(interfacePhosphorus[i].timer <= 0) {
			interfacePhosphorus.erase(interfacePhosphorus.begin());
			continue;
		}

		interfacePhosphorus[i].color.a = (unsigned char)interfacePhosphorus[i].timer;
	}

    // Players
    for(size_t i = 0; i < numPlayers; i++) {
        updatePlayer(players[i], monitorRunning);
        updatePlayerExplosion(players[i], monitorRunning);
        updatePlayerLaser(players[i], monitorRunning);
        updatePlayerLaserExplosion(players[i], monitorRunning);
    }
}

void Phosphorus::draw(bool monitorRunning) {
    // Player/Anomaly
    for(size_t i = 0; i < entitiesPhosphorus.size(); i++) {
        DrawTexturePro(
            entitiesPhosphorus[i].texture,
            entitiesPhosphorus[i].sprite,
            (Rectangle){
                entitiesPhosphorus[i].pos.x,
                entitiesPhosphorus[i].pos.y,
                entitiesPhosphorus[i].sprite.width * entitiesPhosphorus[i].scale,
                entitiesPhosphorus[i].sprite.height * entitiesPhosphorus[i].scale
            },
            {
                entitiesPhosphorus[i].sprite.width * entitiesPhosphorus[i].scale / 2.f,
                entitiesPhosphorus[i].sprite.height * entitiesPhosphorus[i].scale / 2.f
            },
            entitiesPhosphorus[i].angle,
            entitiesPhosphorus[i].color
        );
    }

    // Player explosion
    for(size_t i = 0; i < playerExplosionPhosphorus.size(); i++) {
		for(size_t j = 0; j < LARGE_EXPLOSION_PARTICLES; j++) {
            DrawPixel(
                playerExplosionPhosphorus[i].pos[j].x,
                playerExplosionPhosphorus[i].pos[j].y,
                playerExplosionPhosphorus[i].color
            );
        }
	}

    // Player projectiles
    for(size_t i = 0; i < playerLaserPhosphorus.size(); i++) {
		for(size_t j = 0; j < playerLaserPhosphorus[i].size(); j++) {
            DrawTexturePro(
                playerLaserPhosphorus[i][j].texture,
                playerLaserPhosphorus[i][j].sprite,
                (Rectangle){
                    playerLaserPhosphorus[i][j].pos.x,
                    playerLaserPhosphorus[i][j].pos.y,
                    playerLaserPhosphorus[i][j].sprite.width * playerLaserPhosphorus[i][j].scale,
                    playerLaserPhosphorus[i][j].sprite.height * playerLaserPhosphorus[i][j].scale
                },
                (Vector2){
                    playerLaserPhosphorus[i][j].sprite.width * playerLaserPhosphorus[i][j].scale / 2.f,
                    playerLaserPhosphorus[i][j].sprite.height * playerLaserPhosphorus[i][j].scale / 2.f
                },
                playerLaserPhosphorus[i][j].angle,
                playerLaserPhosphorus[i][j].color
            );
        }
    }

    //Player projectiles explosion
    for(size_t i = 0; i < playerLaserExplosionPhosphorus.size(); i++) {
		for(size_t j = 0; j < playerLaserExplosionPhosphorus[i].size(); j++) {
            for(size_t k = 0; k < SMALL_EXPLOSION_PARTICLES; k++) {
                DrawPixel(
                    playerLaserExplosionPhosphorus[i][j].pos[k].x,
                    playerLaserExplosionPhosphorus[i][j].pos[k].y,
                    playerLaserExplosionPhosphorus[i][j].color
                );
            }
        }
    }

    // Interface
    if(!monitorRunning) {
        return;
    }

    for(size_t i = 0; i < interfacePhosphorus.size(); i++) {
        Vector2 dim = MeasureTextEx(
            interfacePhosphorus[i].font,
            interfacePhosphorus[i].text.c_str(),
            interfacePhosphorus[i].fontSize,
            0.f
        );
        DrawTextEx(
            interfacePhosphorus[i].font,
            interfacePhosphorus[i].text.c_str(),
            {
                interfacePhosphorus[i].pos.x - dim.x / 2,
                interfacePhosphorus[i].pos.y - dim.y / 2
            },
            interfacePhosphorus[i].fontSize,
            0.f,
            interfacePhosphorus[i].color
        );
    }
}