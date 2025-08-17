#include <GameEngine.hpp>

GameEngine::GameEngine(void) :
icon(LoadImage((gamePath + "/res/img/logo.png").c_str())),
knobTex(LoadTexture((gamePath + "/res/img/knob.png").c_str())),
trademarkTex(LoadTexture((gamePath + "/res/img/trademark.png").c_str())),
backgroundTex(LoadTexture((gamePath + "/res/img/space.png").c_str())),
anomalyTex(LoadTexture((gamePath + "/res/img/anomaly.png").c_str())),
playersTex{
    LoadTexture((gamePath + "/res/img/player_1.png").c_str()),
    LoadTexture((gamePath + "/res/img/player_2.png").c_str()),
    LoadTexture((gamePath + "/res/img/player_3.png").c_str()),
    LoadTexture((gamePath + "/res/img/player_4.png").c_str())
},
lasersTex{
    LoadTexture((gamePath + "/res/img/laser.png").c_str()),
    {},
    {},
    {}
},
interfaceFont(LoadFontEx((gamePath + "/res/fonts/Doto.ttf").c_str(), UI_FONT_SIZE, 0, 0)),
laserSound(LoadSound((gamePath + "/res/audio/laser.wav").c_str())),
explosionSound(LoadSound((gamePath + "/res/audio/explosion.wav").c_str())),
monitor(VT_TRADEMARK_POS, trademarkTex, VT_TRADEMARK_SCALE),
flickeringKnob(KNOB_FLCK_POS, knobTex, KNOB_TEX_SCALE),
burnInKnob(KNOB_BRIN_POS, knobTex, KNOB_TEX_SCALE),
userInterface(
	UI_POS,
    interfaceFont,
    UI_TEXT_COLOR
),
background(backgroundTex),
anomaly(
    ANOMALY_POS,
    anomalyTex,
    WINDOW_WIDTH / WSCALE
),
players{
    Player(
        PLAYER_1_START_POS,
        playersTex[0],
        lasersTex[0],
        PLAYER_1_START_ANGLE,
        PLAYER_TEX_SCALE,
        laserSound,
        explosionSound
    ),
    Player(
        PLAYER_2_START_POS,
        playersTex[1],
        lasersTex[0],
        PLAYER_2_START_ANGLE,
        PLAYER_TEX_SCALE,
        laserSound,
        explosionSound
    ),
    Player(
        PLAYER_3_START_POS,
        playersTex[2],
        lasersTex[0],
        PLAYER_3_START_ANGLE,
        PLAYER_TEX_SCALE,
        laserSound,
        explosionSound
    ),
    Player(
        PLAYER_4_START_POS,
        playersTex[3],
        lasersTex[0],
        PLAYER_4_START_ANGLE,
        PLAYER_TEX_SCALE,
        laserSound,
        explosionSound
    )
},
phosphorus(players, anomaly, userInterface) {
    // Setting knobs angle value
    flickeringKnob.setAngle((__flickeringEffectValue - 255) * 180.f / (FLICKERING_MIN_ALPHA - 255));
    burnInKnob.setAngle(__burnInEffectValue * 180.f / 255);

    // Setting bilinear texture filter for better texture quality
    SetTextureFilter(knobTex, TEXTURE_FILTER_BILINEAR);
    SetTextureFilter(trademarkTex, TEXTURE_FILTER_BILINEAR);
    SetTextureFilter(backgroundTex, TEXTURE_FILTER_BILINEAR);
    SetTextureFilter(anomalyTex, TEXTURE_FILTER_BILINEAR);
    for(int i = 0; i < MAX_PLAYERS; i++) {
        SetTextureFilter(playersTex[i], TEXTURE_FILTER_BILINEAR);
    }
    SetTextureFilter(lasersTex[0], TEXTURE_FILTER_BILINEAR);
}

GameEngine::~GameEngine() {
    // Unloading sounds
    UnloadSound(explosionSound);
    UnloadSound(laserSound);

    // Unloading fonts
    UnloadFont(interfaceFont);

    // Unloading textures
    for(int i = 0; i < MAX_PLAYERS; i++) {
        UnloadTexture(playersTex[i]);
        if(i == 0 || !retroStyleShips) {
            UnloadTexture(lasersTex[i]);
        }
    }
    UnloadTexture(anomalyTex);
    UnloadTexture(backgroundTex);
    UnloadTexture(trademarkTex);
    UnloadTexture(knobTex);
    UnloadImage(icon);

    // Closing subsystems
    CloseAudioDevice();
    CloseWindow();
}

void GameEngine::eventsHandler(void) {
    // Start/stop game
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

    // Checking is any knob was selected
    if(IsMouseButtonDown(0)) {
        flickeringKnob.update(GetMousePosition(), updateFlickeringEffectValue);
        burnInKnob.update(GetMousePosition(), updateBurnInEffectValue);
    }

    if(!monitor.isRunning()) {
        // Game mechanics modifiers
        if(IsKeyPressed(KEY_ONE)) {
            // Unloading old textures
            for(int i = 0; i < MAX_PLAYERS; i++) {
                if(i == 0 || !retroStyleShips) {
                    UnloadTexture(lasersTex[i]);
                }
                UnloadTexture(playersTex[i]);
            }

            retroStyleShips = !retroStyleShips;

            // Loading new textures based on selected settings
            if(retroStyleShips) {
                playersTex[0] = LoadTexture((gamePath + "/res/img/player_1.png").c_str());
                playersTex[1] = LoadTexture((gamePath + "/res/img/player_2.png").c_str());
                playersTex[2] = LoadTexture((gamePath + "/res/img/player_3.png").c_str());
                playersTex[3] = LoadTexture((gamePath + "/res/img/player_4.png").c_str());
                lasersTex[0] = LoadTexture((gamePath + "/res/img/laser.png").c_str());

                for(int i = 0; i < MAX_PLAYERS; i++) {
                    players[i].reloadTextures(playersTex[i], lasersTex[0]);
                }
            } else {
                playersTex[0] = LoadTexture((gamePath + "/res/img/player_blue.png").c_str());
                playersTex[1] = LoadTexture((gamePath + "/res/img/player_red.png").c_str());
                playersTex[2] = LoadTexture((gamePath + "/res/img/player_empire.png").c_str());
                playersTex[3] = LoadTexture((gamePath + "/res/img/player_rebellion.png").c_str());
                lasersTex[0] = LoadTexture((gamePath + "/res/img/laser_blue.png").c_str());
                lasersTex[1] = LoadTexture((gamePath + "/res/img/laser_red.png").c_str());
                lasersTex[2] = LoadTexture((gamePath + "/res/img/laser_green.png").c_str());
                lasersTex[3] = LoadTexture((gamePath + "/res/img/laser_magenta.png").c_str());

                for(int i = 0; i < MAX_PLAYERS; i++) {
                    players[i].reloadTextures(playersTex[i], lasersTex[i]);
                }
            }

            // Resetting bilinear texture filter for new loaded textures
            for(int i = 0; i < MAX_PLAYERS; i++) {
                SetTextureFilter(playersTex[i], TEXTURE_FILTER_BILINEAR);
                if(i == 0 || !retroStyleShips) {
                    SetTextureFilter(lasersTex[i], TEXTURE_FILTER_BILINEAR);
                }
            }
        }

        if(IsKeyPressed(KEY_TWO)) {
            shipProjectilesLimit = !shipProjectilesLimit;
			oneShotOneKill = false;
        }

        if(IsKeyPressed(KEY_THREE)) {
            shipFuelLimit = !shipFuelLimit;
        }

        if(IsKeyPressed(KEY_FOUR)) {
            blackHoleAsAnomaly = !blackHoleAsAnomaly;
            anomaly.changeAnomaly();
        }

        if(IsKeyPressed(KEY_FIVE)) {
            oneShotOneKill = !oneShotOneKill;
            shipProjectilesLimit = true;

            for(size_t i = 0; i < numPlayers; i++) {
                players[i].reset();
            }
        }

        if(IsKeyPressed(KEY_SIX)) {
            numPlayers = (numPlayers - MIN_PLAYERS + 1) % (MAX_PLAYERS - MIN_PLAYERS + 1) + MIN_PLAYERS;
        }
    } else {
        // Players mechanics commands
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
	if(__burnInEffectValue > PHOSPHORUS_EFFECT_TRIG) {
		phosphorus.update(monitor.isRunning());
	}

	if(!monitor.isRunning()) {
        userInterface.update(
            "                    XSPACEWAR!\n"
            "                 by Nicolo' Maffi\n\n"

            "System Keys:\n"
            "[ENTER]                Start/Reset game\n"
            "[ESC]                  Quit\n\n"

            "Spaceship Keys:\n"
            "[W] [UP    ] [T] [I]   Shoot\n"
            "[S] [DOWN  ] [G] [K]   Turbo\n"
            "[A] [LEFT  ] [F] [J]   Left rotation\n"
            "[D] [RIGHT ] [H] [L]   Right rotation\n"
            "[E] [RSHIFT] [Y] [O]   Hyperspace\n\n"

            "Game Modifiers:\n"
            "[1]                    Retro' style ships      $f\n"
            "[2]                    Unlimited projectiles   $f\n"
            "[3]                    Unlimited fuel          $f\n"
            "[4]                    Star as anomaly         $f\n"
            "[5]                    One shot One kill       $f\n"
            "[6]                    Number of players       $p\n\n",
            retroStyleShips,
            !shipProjectilesLimit,
            !shipFuelLimit,
            !blackHoleAsAnomaly,
            oneShotOneKill,
            numPlayers
        );
	} else {
        int alive = 0;

        // Counting how many players are alive
        for(size_t i = 0; i < numPlayers; i++) {
            players[i].update(players, i, anomaly);
            alive += !players[i].isDead();
        }

        // If a single player is alive, or all players are dead the game starts again
        if(alive <= 1) {
            for(size_t i = 0; i < numPlayers; i++) {
                players[i].reset();
            }
        }
	}
}

void GameEngine::draw(void) {
    // Drawing burn-in effect
	if(__burnInEffectValue > PHOSPHORUS_EFFECT_TRIG) {
		phosphorus.draw(monitor.isRunning());
	}

	if(monitor.isRunning()) {
        // Drawing game objects
		background.draw();
		anomaly.draw();
		for(size_t i = 0; i < numPlayers; i++) {
            players[i].draw();
        }
	} else {
        // Drawing user interface
		userInterface.draw();
	}

    // Drawing video terminal
	monitor.draw();

    // Drawing the knobs
    flickeringKnob.draw();
    burnInKnob.draw();
}

void GameEngine::gameLoop(void) {
    // Setting FPS and window icon
    SetWindowIcon(icon);
	SetTargetFPS(FPS);

    // Game loop
	while(!WindowShouldClose()) {
		eventsHandler();

		update();

		BeginDrawing();
        ClearBackground(VT_MONITOR_COLOR);

		draw();

		EndDrawing();
	}
}
