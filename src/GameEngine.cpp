#include <GameEngine.hpp>

GameEngine::GameEngine(void) :
laserSound(LoadSound((gamePath + "/res/audio/laser.wav").c_str())),
explosionSound(LoadSound((gamePath + "/res/audio/explosion.wav").c_str())),
monitor((gamePath + "/res/fonts/GoogleSansCode.ttf").c_str()),
userInterface(
	UI_POS,
    (gamePath + "/res/fonts/VT323.ttf").c_str(),
	UI_FONT_SIZE * WINDOW_WIDTH / WSCALE,
    UI_TEXT_COLOR
),
background((gamePath + "/res/img/space.png").c_str()),
players{
    Player(
        PLAYER_1_START_POS,
        (gamePath + "/res/img/player_1.png").c_str(),
        (gamePath + "/res/img/laser.png").c_str(),
        PLAYER_1_START_ANGLE,
        PLAYER_TEX_SCALE,
        laserSound,
        explosionSound
    ),
    Player(
        PLAYER_2_START_POS,
        (gamePath + "/res/img/player_2.png").c_str(),
        (gamePath + "/res/img/laser.png").c_str(),
        PLAYER_2_START_ANGLE,
        PLAYER_TEX_SCALE,
        laserSound,
        explosionSound
    ),
    Player(
        PLAYER_3_START_POS,
        (gamePath + "/res/img/player_3.png").c_str(),
        (gamePath + "/res/img/laser.png").c_str(),
        PLAYER_3_START_ANGLE,
        PLAYER_TEX_SCALE,
        laserSound,
        explosionSound
    ),
    Player(
        PLAYER_4_START_POS,
        (gamePath + "/res/img/player_4.png").c_str(),
        (gamePath + "/res/img/laser.png").c_str(),
        PLAYER_4_START_ANGLE,
        PLAYER_TEX_SCALE,
        laserSound,
        explosionSound
    )
},
anomaly(
    ANOMALY_POS,
    (gamePath + "/res/img/anomaly.png").c_str(),
    WINDOW_WIDTH / WSCALE
),
phosphorus(players, anomaly, userInterface) {
    icon = LoadImage((gamePath + "/res/logo.png").c_str());
    SetWindowIcon(icon);

	loadData();

    for(size_t i = 0; i < numPlayers; i++) {
        players[i].reset();
    }

	anomaly.changeAnomaly();
}

GameEngine::~GameEngine() {
    UnloadSound(laserSound);
    UnloadSound(explosionSound);

    UnloadImage(icon);

    CloseAudioDevice();
    CloseWindow();

    saveData();
}

void GameEngine::loadData(void) {
	std::ifstream stream("./data/data.bin", std::ios::binary);

	stream.read((char *)&burnInMonitorEffect, sizeof(bool));
	stream.read((char *)&flickeringMonitorEffect, sizeof(bool));
	stream.read((char *)&shipProjectilesLimit, sizeof(bool));
	stream.read((char *)&shipFuelLimit, sizeof(bool));
	stream.read((char *)&blackHoleAsAnomaly, sizeof(bool));
	stream.read((char *)&retroStyleShips, sizeof(bool));
	stream.read((char *)&oneShotOneKill, sizeof(bool));
    stream.read((char *)&playSounds, sizeof(bool));
    stream.read((char *)&numPlayers, sizeof(int));

	stream.close();
}

void GameEngine::saveData(void) {
    std::ofstream stream("./data/data.bin", std::ios::binary);

	stream.write((char *)&burnInMonitorEffect, sizeof(bool));
	stream.write((char *)&flickeringMonitorEffect, sizeof(bool));
	stream.write((char *)&shipProjectilesLimit, sizeof(bool));
	stream.write((char *)&shipFuelLimit, sizeof(bool));
	stream.write((char *)&blackHoleAsAnomaly, sizeof(bool));
	stream.write((char *)&retroStyleShips, sizeof(bool));
	stream.write((char *)&oneShotOneKill, sizeof(bool));
    stream.write((char *)&playSounds, sizeof(bool));
    stream.write((char *)&numPlayers, sizeof(int));

	stream.close();
}

void GameEngine::eventsHandler(void) {
    if(IsKeyPressed(KEY_ENTER)) {
        if(monitor.isRunning()) {
            for(size_t i = 0; i < numPlayers; i++) {
                players[i].reset();
            }

            monitor.powerOff();
        } else {
            monitor.powerOn();
        }
    }

    if(!monitor.isRunning()) {
        if(IsKeyPressed(KEY_ONE)) {
            burnInMonitorEffect = !burnInMonitorEffect;
        }

        if(IsKeyPressed(KEY_TWO)) {
            flickeringMonitorEffect = !flickeringMonitorEffect;
        }

        if(IsKeyPressed(KEY_THREE)) {
            retroStyleShips = !retroStyleShips;

            if (retroStyleShips) {
                players[0].reloadTextures((gamePath + "/res/img/player_1.png").c_str(), (gamePath + "/res/img/laser.png").c_str());
                players[1].reloadTextures((gamePath + "/res/img/player_2.png").c_str(), (gamePath + "/res/img/laser.png").c_str());
                players[2].reloadTextures((gamePath + "/res/img/player_3.png").c_str(), (gamePath + "/res/img/laser.png").c_str());
                players[3].reloadTextures((gamePath + "/res/img/player_4.png").c_str(), (gamePath + "/res/img/laser.png").c_str());
            } else {
                players[0].reloadTextures((gamePath + "/res/img/blue_player.png").c_str(), (gamePath + "/res/img/blue_laser.png").c_str());
                players[1].reloadTextures((gamePath + "/res/img/red_player.png").c_str(), (gamePath + "/res/img/red_laser.png").c_str());
                players[2].reloadTextures((gamePath + "/res/img/player_empire.png").c_str(), (gamePath + "/res/img/green_laser.png").c_str());
                players[3].reloadTextures((gamePath + "/res/img/player_rebellion.png").c_str(), (gamePath + "/res/img/magenta_laser.png").c_str());
            }
        }

        if(IsKeyPressed(KEY_FOUR)) {
            playSounds = !playSounds;
        }

        if(IsKeyPressed(KEY_L)) {
            shipProjectilesLimit = !shipProjectilesLimit;

			oneShotOneKill = false;
        }

        if(IsKeyPressed(KEY_F)) {
            shipFuelLimit = !shipFuelLimit;
        }

        if(IsKeyPressed(KEY_S)) {
            blackHoleAsAnomaly = !blackHoleAsAnomaly;

            anomaly.changeAnomaly();
        }

        if(IsKeyPressed(KEY_K)) {
            oneShotOneKill = !oneShotOneKill;

            shipProjectilesLimit = true;

            for(size_t i = 0; i < numPlayers; i++) {
                players[i].reset();
            }
        }

        if(IsKeyPressed(KEY_P)) {
            numPlayers = (numPlayers - MIN_PLAYERS + 1) % (MAX_PLAYERS - MIN_PLAYERS + 1) + MIN_PLAYERS;
        }
    } else {
        for(size_t i = 0; i < numPlayers; i++) {
            if(IsKeyDown(keyboardMap[i][0])) {
                players[i].move(PLAYER_SPRINT);
            }

            if(IsKeyPressed(keyboardMap[i][1])) {
                players[i].shoot();
            }

            if(IsKeyPressed(keyboardMap[i][2])) {
                players[i].hyperspace();
            }

            if(IsKeyDown(keyboardMap[i][3])) {
                players[i].rotate(-PLAYER_ROTATION_SPEED);
            }

            if(IsKeyDown(keyboardMap[i][4])) {
                players[i].rotate(PLAYER_ROTATION_SPEED);
            }
        }
    }
}

void GameEngine::update(void) {
	if(burnInMonitorEffect) {
		phosphorus.update(monitor.isRunning());
	}

	if(!monitor.isRunning()) {
        userInterface.update(
            "                     XSPACEWAR!\n"
            "                  by Nicolo' Maffi\n\n"

            "System Keys:\n"
            "[ENTER]               Start/Reset game\n"
            "[ESC]                 Quit\n\n"

            "Spaceship Keys:\n"
            "[W] [UP]    [T] [I]   Shoot\n"
            "[S] [DOWN]  [G] [K]   Turbo\n"
            "[A] [LEFT]  [F] [J]   Left rotation\n"
            "[D] [RIGHT] [H] [L]   Right rotation\n"
            "[E] [RCTRL] [Y] [O]   Hyperspace\n\n"

            "Appearance:\n"
            "[1]                   Burn-in monitor effect     $f\n"
            "[2]                   Flickering monitor effect  $f\n"
            "[3]                   Retro' style ships         $f\n"
            "[4]                   Sounds                     $f\n\n"

            "Game Modifiers:\n"
            "[L]                   Unlimited projectiles      $f\n"
            "[F]                   Unlimited fuel             $f\n"
            "[S]                   Star as anomaly            $f\n"
            "[K]                   One shot One kill          $f\n"
            "[P]                   Number of players          $p\n\n",
            burnInMonitorEffect,
            flickeringMonitorEffect,
            retroStyleShips,
            playSounds,
            !shipProjectilesLimit,
            !shipFuelLimit,
            !blackHoleAsAnomaly,
            oneShotOneKill,
            numPlayers
        );
	} else {
        int alive = 0;

        for(size_t i = 0; i < numPlayers; i++) {
            players[i].update(players, i, anomaly);
            alive += !players[i].isDead();
        }

        if(alive <= 1) {
            for(size_t i = 0; i < numPlayers; i++) {
                players[i].reset();
            }
        }
	}
}

void GameEngine::draw(void) {
	if(burnInMonitorEffect) {
		phosphorus.draw(monitor.isRunning());
	}

	if(monitor.isRunning()) {
		background.draw();
		anomaly.draw();
		for(size_t i = 0; i < numPlayers; i++) {
            players[i].draw();
        }
	} else {
		userInterface.draw();
	}

	monitor.draw();
}

void GameEngine::gameLoop(void) {
	SetTargetFPS(FPS);

	while(!WindowShouldClose()) {
		eventsHandler();

		update();

		BeginDrawing();
        ClearBackground(VT_MONITOR_COLOR);

		draw();

		EndDrawing();
	}
}
