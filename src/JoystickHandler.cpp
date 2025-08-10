#include <JoystickHandler.hpp>

JoystickHandler::JoystickHandler(void): joysticks{NULL}, event({}) {
    SDL_InitSubSystem(SDL_INIT_JOYSTICK);
    SDL_JoystickEventState(SDL_ENABLE);
}

JoystickHandler::~JoystickHandler() {
    for(int i = 0; i < MAX_PLAYERS; i++) {
        if(joysticks[i]) {
            SDL_JoystickClose(joysticks[i]);
        }
    }

    SDL_QuitSubSystem(SDL_INIT_JOYSTICK);
}

void JoystickHandler::addJoystick(SDL_JoystickID dev) {
    if(dev < 0 || dev >= SDL_NumJoysticks()) {
        return;
    }

    for(int i = 0; i < MAX_PLAYERS; i++) {
        if(joysticks[i] != NULL) {
            continue;
        }

        joysticks[i] = SDL_JoystickOpen(dev);
        break;
    }
}

void JoystickHandler::removeJoystick(SDL_JoystickID dev) {
    for(int i = 0; i < MAX_PLAYERS; i++) {
        if(joysticks[i] && SDL_JoystickInstanceID(joysticks[i]) != dev) {
            continue;
        }

        SDL_JoystickClose(joysticks[i]);
        joysticks[i] = NULL;
        break;
    }
}

int JoystickHandler::getPos(SDL_JoystickID dev) {
    int pos = -1;

    for(int i = 0; i < MAX_PLAYERS && pos == -1; i++) {
        if(joysticks[i] && SDL_JoystickInstanceID(joysticks[i]) == dev) {
            pos = i;
        }
    }

    return pos;
}

void JoystickHandler::handler(Player *players, const unsigned *configs) {
    int pos = -1;

    while(SDL_PollEvent(&event)) {
        switch(event.type) {
            case SDL_JOYDEVICEADDED:
                addJoystick(event.jdevice.which);
                break;
            
            case SDL_JOYDEVICEREMOVED:
                removeJoystick(event.jdevice.which);
                break;

            case SDL_JOYBUTTONDOWN:
                pos = getPos(event.jbutton.which);

                if(event.jbutton.button == joystickMap[configs[pos]][3]) {
                    players[pos].shoot();
                } else if(event.jbutton.button == joystickMap[configs[pos]][4]) {
                    players[pos].hyperspace();
                }
                break;
        }
    }

    for(size_t i = 0; i < numPlayers; i++) {
        if(SDL_JoystickGetButton(joysticks[i], joystickMap[configs[i]][0])) {
            players[i] .move(SHIP_SPRINT);
        }
    }

    for(size_t i = 0; i < numPlayers; i++) {
        int value = SDL_JoystickGetAxis(joysticks[i], 0);

        if(value < -joystickMap[configs[i]][1]) {
            players[i].rotate(-SHIP_ROTATION_SPEED);
        } else if(value > joystickMap[configs[i]][2]) {
            players[i].rotate(SHIP_ROTATION_SPEED);
        }
    }
}