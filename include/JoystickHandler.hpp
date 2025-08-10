#ifndef __JOYSTICKHANDLER_HPP__
#define __JOYSTICKHANDLER_HPP__

#include <xspacewar.hpp>
#include <Player.hpp>

#include <SDL2/SDL.h>

class JoystickHandler {
private:
    SDL_Joystick *joysticks[MAX_PLAYERS];
    SDL_Event event;

    void addJoystick(SDL_JoystickID dev);
    void removeJoystick(SDL_JoystickID dev);
    int getPos(SDL_JoystickID dev);
public:
    JoystickHandler(void);
    ~JoystickHandler();
    void handler(Player *players, const unsigned *configs);
};

#endif //__JOYSTICKHANDLER_HPP__