#include <Phosphorus.hpp>

GhostEntity::GhostEntity(const Player &player):
pos(player.pos),
angle(player.angle),
texture(player.texture),
sprite(player.sprite),
color(PHOSPHORUS_COLOR(255)),
scale(player.scale),
timer(__burnInEffectValue) {
}

GhostEntity::GhostEntity(const Laser &laser):
pos(laser.pos),
angle(laser.angle),
texture(laser.texture),
sprite((Rectangle){.0f, .0f, (float)laser.texture.width, (float)laser.texture.height}),
color(PHOSPHORUS_COLOR(255)),
scale(laser.scale),
timer(__burnInEffectValue) {
}

GhostEntity::GhostEntity(const Anomaly &anomaly):
pos(anomaly.pos),
angle(anomaly.angle),
texture(anomaly.texture),
sprite(anomaly.sprite),
color(PHOSPHORUS_COLOR(255)),
scale(anomaly.scale),
timer(__burnInEffectValue) {
}

GhostParticles::GhostParticles(const Particles &particles, size_t size):
pos(particles.pos, particles.pos + size),
color(PHOSPHORUS_COLOR(255)),
timer(__burnInEffectValue) {
}

GhostInterface::GhostInterface(const Interface &interface):
pos(interface.pos),
text(interface.text),
font(interface.font),
color(PHOSPHORUS_COLOR(255)),
timer(__burnInEffectValue) {
}

Phosphorus::Phosphorus(const Player *players, const Anomaly &anomaly, const Interface &interface):
players(players),
anomaly(anomaly),
interface(interface) {
}

void Phosphorus::updatePlayer(const Player &player, bool monitorRunning) {
    if(monitorRunning && !player.isInHyperspace() && !player.isExploding() && !player.isCapturedByGravityWell() && !player.isDead()) {
        // Pushing a copy of the player texture to simulating the burn-in effect
        entitiesPhosphorus.push_back(GhostEntity(player));
    }

    // Updating all player "ghost textures" alpha value
    size_t i = 0;
    while(i < entitiesPhosphorus.size()) {
        // Updating the alpha timer value
        entitiesPhosphorus[i].timer -= GetFrameTime() * PHOSPHORUS_DT_MULTIPLIER;

        // If the timer reaches zero the "ghost texture" must be deleted
        if(entitiesPhosphorus[i].timer <= .0f) {
            entitiesPhosphorus.erase(entitiesPhosphorus.begin() + i);
        } else {
            // Updating "ghost texture" color alpha value
            entitiesPhosphorus[i].color.a = (unsigned char)entitiesPhosphorus[i].timer;
            i++;
        }
    }
}

void Phosphorus::updatePlayerExplosion(const Player &player, bool monitorRunning) {
    if(monitorRunning && player.explosionTimer >= EXPLOSION_LARGE_TIME && player.isExploding() &&
    !player.isCapturedByGravityWell() && !player.isDead()) {
        // Pushing a copy of the explosion particles array
		playerExplosionPhosphorus.push_back(GhostParticles(player.particles, EXPLOSION_LARGE_PARTICLES));
	}

    // Updating all "ghost particles" alpha value
    size_t i = 0;
	while(i < playerExplosionPhosphorus.size()) {
		playerExplosionPhosphorus[i].timer -= GetFrameTime() * PHOSPHORUS_DT_MULTIPLIER;

		if(playerExplosionPhosphorus[i].timer <= 0) {
			playerExplosionPhosphorus.erase(playerExplosionPhosphorus.begin() + i);
		} else {
            playerExplosionPhosphorus[i].color.a = (unsigned char)playerExplosionPhosphorus[i].timer;
            i++;
        }
	}
}

void Phosphorus::updatePlayerLaser(const Player &player, bool monitorRunning) {
    for(size_t i = playerLaserPhosphorus.size(); i < player.projectiles.size(); i++) {
        // Pushing a new std::vector<> for the player lasers
        playerLaserPhosphorus.push_back(std::vector<GhostEntity>());
    }

    // Pushing a copy of all the player lasers textures
    for(size_t i = 0; i < player.projectiles.size(); i++) {
        if(monitorRunning && !player.projectiles[i].isExploding()) {
			playerLaserPhosphorus[i].push_back(GhostEntity(player.projectiles[i]));
		}
	}

    // Updating all player lasers "ghost textures" alpha value
    for(size_t i = 0; i < playerLaserPhosphorus.size(); i++) {
        size_t j = 0;
        while(j < playerLaserPhosphorus[i].size()) {
            playerLaserPhosphorus[i][j].timer -= GetFrameTime() * PHOSPHORUS_DT_MULTIPLIER;

            if(playerLaserPhosphorus[i][j].timer <= .0f) {
                playerLaserPhosphorus[i].erase(playerLaserPhosphorus[i].begin() + j);
            } else {
                playerLaserPhosphorus[i][j].color.a = (unsigned char)playerLaserPhosphorus[i][j].timer;
                j++;
            }
        }
    }

    size_t i =0;
    while(i < playerLaserPhosphorus.size()) {
        // Deleting the std::vector<> if is empty
        if(playerLaserPhosphorus[i].empty()) {
            playerLaserPhosphorus.erase(playerLaserPhosphorus.begin() + i);
        } else {
            i++;
        }
    }
}

void Phosphorus::updatePlayerLaserExplosion(const Player &player, bool monitorRunning) {
    for(size_t i = playerLaserExplosionPhosphorus.size(); i < player.projectiles.size(); i++) {
        // Pushing a new std::vector<> for the player lasers particles
        playerLaserExplosionPhosphorus.push_back(std::vector<GhostParticles>());
    }

    // Pushing a copy of all the lasers explosion particles arrays
	for(size_t i = 0; i < player.projectiles.size(); i++) {
		if(monitorRunning && player.projectiles[i].explosionTimer >= EXPLOSION_SMALL_TIME &&
            player.projectiles[i].isExploding()) {
			playerLaserExplosionPhosphorus[i].push_back(
                GhostParticles(player.projectiles[i].particles, EXPLOSION_SMALL_PARTICLES)
            );
		}
	}

    // Updating all players lasers explosion "ghost particles" alpha value
    for(size_t i = 0; i < playerLaserExplosionPhosphorus.size(); i++) {
        size_t j = 0;
        while(j < playerLaserExplosionPhosphorus[i].size()) {
            playerLaserExplosionPhosphorus[i][j].timer -= GetFrameTime() * PHOSPHORUS_DT_MULTIPLIER;

            if(playerLaserExplosionPhosphorus[i][j].timer <= .0f) {
                playerLaserExplosionPhosphorus[i].erase(playerLaserExplosionPhosphorus[i].begin() + j);
            } else {
                playerLaserExplosionPhosphorus[i][j].color.a = (unsigned char)playerLaserExplosionPhosphorus[i][j].timer;
                j++;
            }
        }
    }

    size_t i = 0;
    while(i < playerLaserExplosionPhosphorus.size()) {
        // Deleting the std::vector<> if is empty
        if(playerLaserExplosionPhosphorus[i].empty()) {
            playerLaserExplosionPhosphorus.erase(playerLaserExplosionPhosphorus.begin() + i);
        } else {
            i++;
        }
    }
}

void Phosphorus::update(bool monitorRunning) {
    // Updating the anomaly burn-in effect
	if(monitorRunning) {
		entitiesPhosphorus.push_back(GhostEntity(anomaly));
	}

    // Updating the interface text
    if(!monitorRunning) {
		interfacePhosphorus.push_back(GhostInterface(interface));
	}

    // Updating all interface "ghost texts" alpha value
    size_t i = 0;
	while(i < interfacePhosphorus.size()) {
		interfacePhosphorus[i].timer -= GetFrameTime() * DT_MULTIPLIER;

		if(interfacePhosphorus[i].timer <= 0) {
			interfacePhosphorus.erase(interfacePhosphorus.begin());
		} else {
            interfacePhosphorus[i].color.a = (unsigned char)interfacePhosphorus[i].timer;
            i++;
        }
	}

    // Updating the players
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
		for(size_t j = 0; j < EXPLOSION_LARGE_PARTICLES; j++) {
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
            for(size_t k = 0; k < EXPLOSION_SMALL_PARTICLES; k++) {
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
            interfacePhosphorus[i].font.baseSize,
            0.f
        );
        DrawTextEx(
            interfacePhosphorus[i].font,
            interfacePhosphorus[i].text.c_str(),
            {
                interfacePhosphorus[i].pos.x - dim.x / 2,
                interfacePhosphorus[i].pos.y - dim.y / 2
            },
            interfacePhosphorus[i].font.baseSize,
            0.f,
            interfacePhosphorus[i].color
        );
    }
}
