#include <GameEngine.hpp>

GameEngine::GameEngine(void) :
joystickConfigs {0, 0, 0, 0},
laserSound(LoadSound("./res/audio/laser.wav")),
explosionSound(LoadSound("./res/audio/explosion.wav")),
monitor("./res/img/monitor.png"),
userInterface(
	(Vector2){WINDOW_CENTER_X, WINDOW_CENTER_Y},
    "./res/fonts/VT323.ttf",
	22 * WINDOW_WIDTH / WSCALE,
    TEXT_COLOR
),
background("./res/img/space.png"),
players{
    Player(
        (Vector2){WINDOW_CENTER_X - 290.f * WINDOW_WIDTH / WSCALE, WINDOW_CENTER_Y + 290.f * WINDOW_HEIGHT / HSCALE},
        "./res/img/player_1.png",
        "./res/img/laser.png",
        270.f,
        .8f * WINDOW_WIDTH / WSCALE,
        laserSound,
        explosionSound
    ),
    Player(
        (Vector2){WINDOW_CENTER_X + 290.f * WINDOW_WIDTH / WSCALE , WINDOW_CENTER_Y - 290.f * WINDOW_HEIGHT / HSCALE},
        "./res/img/player_2.png",
        "./res/img/laser.png",
        90.f,
        .8f * WINDOW_WIDTH / WSCALE,
        laserSound,
        explosionSound
    ),
    Player(
        (Vector2){WINDOW_CENTER_X - 290.f * WINDOW_WIDTH / WSCALE , WINDOW_CENTER_Y - 290.f * WINDOW_HEIGHT / HSCALE},
        "./res/img/player_3.png",
        "./res/img/laser.png",
        90.f,
        .8f * WINDOW_WIDTH / WSCALE,
        laserSound,
        explosionSound
    ),
    Player(
        (Vector2){WINDOW_CENTER_X + 290.f * WINDOW_WIDTH / WSCALE , WINDOW_CENTER_Y + 290.f * WINDOW_HEIGHT / HSCALE},
        "./res/img/player_4.png",
        "./res/img/laser.png",
        270.f,
        .8f * WINDOW_WIDTH / WSCALE,
        laserSound,
        explosionSound
    )
},
anomaly(
    (Vector2){WINDOW_CENTER_X, WINDOW_CENTER_Y},
    "./res/img/anomaly.png",
    WINDOW_WIDTH / WSCALE
),
phosphorus(players, anomaly, userInterface) {
    icon = LoadImage("./res/logo.png");
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

	stream.read((char*)&burnInMonitorEffect, sizeof(bool));
	stream.read((char*)&flickeringMonitorEffect, sizeof(bool));
	stream.read((char*)&shipProjectilesLimit, sizeof(bool));
	stream.read((char*)&shipFuelLimit, sizeof(bool));
	stream.read((char*)&blackHoleAsAnomaly, sizeof(bool));
	stream.read((char*)&retroStyleShips, sizeof(bool));
	stream.read((char*)&oneShotOneKill, sizeof(bool));
    stream.read((char*)&playSounds, sizeof(bool));
    stream.read((char*)&numPlayers, sizeof(int));

	for(size_t i = 0; i < MAX_PLAYERS; i++) {
        stream.read((char *)(joystickConfigs + i), sizeof(unsigned));
    }

	stream.close();
}

void GameEngine::saveData(void) {
    std::ofstream stream("./data/data.bin", std::ios::binary);

	stream.write((char*)&burnInMonitorEffect, sizeof(bool));
	stream.write((char*)&flickeringMonitorEffect, sizeof(bool));
	stream.write((char*)&shipProjectilesLimit, sizeof(bool));
	stream.write((char*)&shipFuelLimit, sizeof(bool));
	stream.write((char*)&blackHoleAsAnomaly, sizeof(bool));
	stream.write((char*)&retroStyleShips, sizeof(bool));
	stream.write((char*)&oneShotOneKill, sizeof(bool));
    stream.write((char*)&playSounds, sizeof(bool));
    stream.write((char*)&numPlayers, sizeof(int));

    for(size_t i = 0; i < MAX_PLAYERS; i++) {
        stream.write((char *)(joystickConfigs + i), sizeof(unsigned));
    }

	stream.close();
}

void GameEngine::eventsHandler(void) {
    joystickHandler.handler(players, joystickConfigs);

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
            joystickConfigs[0] = (joystickConfigs[0] + 1) % JOYSTICK_CONFIGURATIONS;
        }

        if(IsKeyPressed(KEY_TWO)) {
            joystickConfigs[1] = (joystickConfigs[1] + 1) % JOYSTICK_CONFIGURATIONS;
        }

        if(IsKeyPressed(KEY_THREE)) {
            joystickConfigs[2] = (joystickConfigs[2] + 1) % JOYSTICK_CONFIGURATIONS;
        }

        if(IsKeyPressed(KEY_FOUR)) {
            joystickConfigs[3] = (joystickConfigs[3] + 1) % JOYSTICK_CONFIGURATIONS;
        }

        if(IsKeyPressed(KEY_F11)) {
            burnInMonitorEffect = !burnInMonitorEffect;
        }

        if(IsKeyPressed(KEY_F10)) {
            flickeringMonitorEffect = !flickeringMonitorEffect;
        }

        if(IsKeyPressed(KEY_F9)) {
            retroStyleShips = !retroStyleShips;

            if (retroStyleShips) {
                players[0].reloadTextures("./res/img/player_1.png", "./res/img/laser.png");
                players[1].reloadTextures("./res/img/player_2.png", "./res/img/laser.png");
                players[2].reloadTextures("./res/img/player_3.png", "./res/img/laser.png");
                players[3].reloadTextures("./res/img/player_4.png", "./res/img/laser.png");
            } else {
                players[0].reloadTextures("./res/img/blue_player.png", "./res/img/blue_laser.png");
                players[1].reloadTextures("./res/img/red_player.png", "./res/img/red_laser.png");
                players[2].reloadTextures("./res/img/player_empire.png", "./res/img/green_laser.png");
                players[3].reloadTextures("./res/img/player_rebellion.png", "./res/img/magenta_laser.png");
            }
        }

        if(IsKeyPressed(KEY_F8)) {
            playSounds = !playSounds;
        }

        if(IsKeyPressed(KEY_F1)) {
            shipProjectilesLimit = !shipProjectilesLimit;

			oneShotOneKill = false;
        }

        if(IsKeyPressed(KEY_F2)) {
            shipFuelLimit = !shipFuelLimit;
        }

        if(IsKeyPressed(KEY_F3)) {
            blackHoleAsAnomaly = !blackHoleAsAnomaly;

            anomaly.changeAnomaly();
        }

        if(IsKeyPressed(KEY_F4)) {
            oneShotOneKill = !oneShotOneKill;

            shipProjectilesLimit = true;

            for(size_t i = 0; i < numPlayers; i++) {
                players[i].reset();
            }
        }

        if(IsKeyPressed(KEY_F5)) {
            numPlayers = (numPlayers - MIN_PLAYERS + 1) % (MAX_PLAYERS - MIN_PLAYERS + 1) + MIN_PLAYERS;
        }
    } else {
        for(size_t i = 0; i < numPlayers; i++) {
            if(IsKeyDown(keyboardMap[i][0])) {
                players[i].move(SHIP_SPRINT);
            }

            if(IsKeyDown(keyboardMap[i][1])) {
                players[i].rotate(-SHIP_ROTATION_SPEED);
            }

            if(IsKeyDown(keyboardMap[i][2])) {
                players[i].rotate(SHIP_ROTATION_SPEED);
            }

            if(IsKeyPressed(keyboardMap[i][3])) {
                players[i].shoot();
            }

            if(IsKeyPressed(keyboardMap[i][4])) {
                players[i].hyperspace();
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
			"                     XSPACEWAR!               \n"
			"                  by Nicolo' Maffi             \n\n"

			"System Keys:\n"
			"[ENTER]               Start/Reset game\n"
			"[ESC]                 Quit\n\n"

			"Spaceship Keys:\n"
			"[W] [UP]    [T] [I]   Shoot\n"
			"[S] [DOWN]  [G] [K]   Turbo\n"
			"[A] [LEFT]  [F] [J]   Left rotation\n"
			"[D] [RIGHT] [H] [L]   Right rotation\n"
			"[E] [CANC]  [Y] [O]   Hyperspace\n\n"

			"Joystick Configuration:\n"
			"[1]                   Player 1         $j\n"
			"[2]                   Player 2         $j\n"
            "[3]                   Player 3         $j\n"
            "[4]                   Player 4         $j\n\n"

			"Appearance:\n"
			"[F11]                 Burn-in monitor effect     $f\n"
			"[F10]                 Flickering monitor effect  $f\n"
			"[F9]                  Retro' style ships         $f\n"
            "[F8]                  Sounds                     $f\n\n"

			"Game Modifiers:\n"
			"[F1]                  Unlimited projectiles      $f\n"
			"[F2]                  Unlimited fuel             $f\n"
			"[F3]                  Star as central point      $f\n"
			"[F4]                  One shot One kill          $f\n"
            "[F5]                  Number of players          $p\n\n",
			joystickConfigs[0],
            joystickConfigs[1],
            joystickConfigs[2],
            joystickConfigs[3],
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
        ClearBackground(CRT_MONITOR_COLOR);

		draw();

		EndDrawing();
	}
}
