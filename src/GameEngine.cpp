#include <GameEngine.hpp>

GameEngine::GameEngine() :
p1jc(0),
p2jc(0),
p1jid(0),
p2jid(1),
monitor("./res/img/monitor.png"),
userInterface(
	(Vector2){WINDOW_CENTER_X, WINDOW_CENTER_Y},
    "./res/fonts/VT323.ttf",
	24 * WINDOW_WIDTH / (unsigned int)WSCALE,
    TEXT_COLOR
),
background("./res/img/space.png"),
player1(
	(Vector2){WINDOW_CENTER_X - 290.f * WINDOW_WIDTH / WSCALE, WINDOW_CENTER_Y + 290.f * WINDOW_HEIGHT / HSCALE},
	"./res/img/player_1.png",
    "./res/img/laser.png",
    270.f,
    .7f * WINDOW_WIDTH / WSCALE
),
player2(
	(Vector2){WINDOW_CENTER_X + 290.f * WINDOW_WIDTH / WSCALE , WINDOW_CENTER_Y - 290.f * WINDOW_HEIGHT / HSCALE},
	"./res/img/player_2.png",
    "./res/img/laser.png",
    90.f,
    .7f * WINDOW_WIDTH / WSCALE
),
anomaly(
    (Vector2){WINDOW_CENTER_X, WINDOW_CENTER_Y},
    "./res/img/anomaly.png",
    WINDOW_WIDTH / WSCALE
),
phosphorus(player1, player2, anomaly, userInterface) {
	loadData();

	player1.reset();
	player2.reset();

	anomaly.changeAnomaly();
}

GameEngine::~GameEngine() {
	std::ofstream stream("./data/data.bin", std::ios::binary);

	stream.write((char*)&burnInMonitorEffect, sizeof(bool));
	stream.write((char*)&flickeringMonitorEffect, sizeof(bool));
	stream.write((char*)&shipProjectilesLimit, sizeof(bool));
	stream.write((char*)&shipFuelLimit, sizeof(bool));
	stream.write((char*)&blackHoleAsAnomaly, sizeof(bool));
	stream.write((char*)&retroStyleShips, sizeof(bool));
	stream.write((char*)&oneShotOneKill, sizeof(bool));

	stream.write((char *)&p1jc, sizeof(unsigned));
	stream.write((char *)&p2jc, sizeof(unsigned));

	stream.close();
}

void GameEngine::loadData() {
	std::ifstream stream("./data/data.bin", std::ios::binary);

	stream.read((char*)&burnInMonitorEffect, sizeof(bool));
	stream.read((char*)&flickeringMonitorEffect, sizeof(bool));
	stream.read((char*)&shipProjectilesLimit, sizeof(bool));
	stream.read((char*)&shipFuelLimit, sizeof(bool));
	stream.read((char*)&blackHoleAsAnomaly, sizeof(bool));
	stream.read((char*)&retroStyleShips, sizeof(bool));
	stream.read((char*)&oneShotOneKill, sizeof(bool));

	stream.read((char *)&p1jc, sizeof(unsigned));
	stream.read((char *)&p2jc, sizeof(unsigned));

	stream.close();
}

void GameEngine::eventsHandler() {
    if(IsKeyPressed(KEY_ENTER)) {
        if(monitor.isRunning()) {
            player1.reset();
            player2.reset();

            monitor.powerOff();
        } else {
            monitor.powerOn();
        }
    }

    if(!monitor.isRunning()) {
        if(IsKeyPressed(KEY_TAB) && !monitor.isRunning()) {
            int tmp = p1jid;
            p1jid = p2jid;
            p2jid = tmp;

            tmp = p1jc;
            p1jc = p2jc;
            p2jc = tmp;
        }

        if(IsKeyPressed(KEY_LEFT_CONTROL)) {
            p1jc = (p1jc + 1) % JOYSTICK_CONFIGURATIONS;
        }

        if(IsKeyPressed(KEY_RIGHT_CONTROL)) {
            p2jc = (p2jc + 1) % JOYSTICK_CONFIGURATIONS;
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
                player1.reloadTextures("./res/img/player_1.png", "./res/img/laser.png");
                player2.reloadTextures("./res/img/player_2.png", "./res/img/laser.png");
            } else {
                player1.reloadTextures("./res/img/blue_player.png", "./res/img/blue_laser.png");
                player2.reloadTextures("./res/img/red_player.png", "./res/img/red_laser.png");
            }
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

            player1.reset();
            player2.reset();
        }
    } else {
        // Player 1
        if(IsKeyPressed(KEY_W) || IsGamepadButtonPressed(p1jid, joystickMap[p1jc][4])) {
            player1.shoot();
        }

        if(IsKeyDown(KEY_A) || GetGamepadAxisMovement(p1jid, 0) < -joystickMap[p1jc][0] / 100.f) {
            player1.rotate(-SHIP_ROTATION_SPEED);
        }

        if(IsKeyDown(KEY_S) || IsGamepadButtonDown(p1jid, joystickMap[p1jc][2])) {
            player1.move(SHIP_SPRINT);
        }

        if(IsKeyDown(KEY_D) || GetGamepadAxisMovement(p1jid, 0) > joystickMap[p1jc][1] / 100.f) {
            player1.rotate(SHIP_ROTATION_SPEED);
        }

        if(IsKeyPressed(KEY_E) || IsGamepadButtonPressed(p1jid, joystickMap[p1jc][3])) {
            player1.hyperspace();
        }

        // Player 2
        if(IsKeyPressed(KEY_UP) || IsGamepadButtonPressed(p2jid, joystickMap[p2jc][4])) {
            player2.shoot();
        }

        if(IsKeyDown(KEY_LEFT) || GetGamepadAxisMovement(p2jid, 0) < -joystickMap[p2jc][0] / 100.f) {
            player2.rotate(-SHIP_ROTATION_SPEED);
        }

        if(IsKeyDown(KEY_DOWN) || IsGamepadButtonDown(p2jid, joystickMap[p2jc][2])) {
            player2.move(SHIP_SPRINT);
        }

        if(IsKeyDown(KEY_RIGHT) || GetGamepadAxisMovement(p2jid, 0) > joystickMap[p2jc][1] / 100.f) {
            player2.rotate(SHIP_ROTATION_SPEED);
        }

        if(IsKeyPressed(KEY_DELETE) || IsGamepadButtonPressed(p2jid, joystickMap[p2jc][3])) {
            player2.hyperspace();
        }
    }
}

void GameEngine::update() {
	if(burnInMonitorEffect) {
		phosphorus.update(monitor.isRunning());
	}

	if(!monitor.isRunning()) {
		userInterface.update(
			"               XSPACEWAR!               \n"
			"            by Nicolo' Maffi             \n\n"

			"System Keys:\n"
			"[ENTER]     Start/Reset game\n"
			"[ESC]       Quit\n\n"

			"Spaceship Keys:\n"
			"[W] [UP]    Shoot\n"
			"[S] [DOWN]  Turbo\n"
			"[A] [LEFT]  Left rotation\n"
			"[D] [RIGHT] Right rotation\n"
			"[E] [CANC]  Hyperspace\n\n"

			"Joystick Configuration:\n"
			"[TAB]       Swap joysticks            \n"
			"[L CTRL]    Player 1                $j\n"
			"[R CTRL]    Player 2                $j\n\n"

			"Appearance:\n"
			"[F11]       Burn-in monitor effect     $f\n"
			"[F10]       Flickering monitor effect  $f\n"
			"[F9]        Retro' style ships         $f\n\n"

			"Game Modifiers:\n"
			"[F1]        Unlimited projectiles      $f\n"
			"[F2]        Unlimited fuel             $f\n"
			"[F3]        Star as central point      $f\n"
			"[F4]        One shot One kill          $f\n\n",
			p1jc,
			p2jc,
			burnInMonitorEffect,
			flickeringMonitorEffect,
			retroStyleShips,
			!shipProjectilesLimit,
			!shipFuelLimit,
			!blackHoleAsAnomaly,
			oneShotOneKill
		);
	} else {
		player1.update(player2, anomaly);
		player2.update(player1, anomaly);
	}
}

void GameEngine::draw() {
	if(burnInMonitorEffect) {
		phosphorus.draw(monitor.isRunning());
	}

	if(monitor.isRunning()) {
		background.draw();
		anomaly.draw();
		player1.draw();
		player2.draw();
	} else {
		userInterface.draw();
	}

	monitor.draw();
}

void GameEngine::gameLoop() {
	SetTargetFPS(FPS);

	while(!WindowShouldClose()) {
		eventsHandler();

		update();

		BeginDrawing();
        ClearBackground(CRT_MONITOR_COLOR);

		draw();

		EndDrawing();
	}

    CloseWindow();
}