#ifndef __XSPACEWAR_HPP__
#define __XSPACEWAR_HPP__

#include <raylib.h>

#include <vector>
#include <cmath>
#include <random>
#include <fstream>

// Globals
#define JOYSTICK_CONFIGURATIONS 1

#define NUM_ACTIONS 5

#define MAX_PLAYERS 4
#define MIN_PLAYERS 2

// Joystick map [turbo, neg trig rotation (%), pos trig rotation (%), shoot, hyperspace]
const int joystickMap[JOYSTICK_CONFIGURATIONS][NUM_ACTIONS] = {
	{0x0C, 0x63, 0x63, 0x08, 0x0B} // PlayStation 4/5
};
const char* const joystickConfigsNames[JOYSTICK_CONFIGURATIONS] = {
	"[PLAYSTATION]"
	//"[WINGMAN    ]",
	//"[GENERIC USB]"
};

// Keyboard map [turbo, left, right, shoot, hyperspace]
const KeyboardKey keyboardMap[MAX_PLAYERS][NUM_ACTIONS] = {
	{KEY_S, KEY_A, KEY_D, KEY_W, KEY_E},
	{KEY_DOWN, KEY_LEFT, KEY_RIGHT, KEY_UP, KEY_RIGHT_CONTROL},
	{KEY_G, KEY_F, KEY_H, KEY_T, KEY_Y},
	{KEY_K, KEY_J, KEY_L, KEY_I, KEY_O},
};

// Global parameters
extern bool   burnInMonitorEffect;
extern bool   flickeringMonitorEffect;
extern bool   shipProjectilesLimit;
extern bool   shipFuelLimit;
extern bool   blackHoleAsAnomaly;
extern bool   retroStyleShips;
extern bool   oneShotOneKill;
extern bool   playSounds;
extern size_t numPlayers;

// Graphics Constants
#define WINDOW_WIDTH ((float)GetMonitorWidth(0))
#define WINDOW_HEIGHT ((float)GetMonitorHeight(0))

#define WSCALE 1920.f
#define HSCALE 1080.f

#define WINDOW_CENTER_X (WINDOW_WIDTH / 2.f)
#define WINDOW_CENTER_Y (WINDOW_HEIGHT / 2.f)

#define MONITOR_RADIUS (WINDOW_HEIGHT / 2.f - 72 * WINDOW_WIDTH / WSCALE)

#define FPS 30
#define MULTIPLIER 60.f
#define FLICKER_FRAME_INTERVAL .08f

// Colors (RGBA)
#define BACKGROUND_COLOR ((Color){34, 34, 34, 255})
#define PHOSPHORUS_COLOR(alpha) ((Color){139, 180, 60, alpha})
#define CRT_MONITOR_COLOR ((Color){34, 42, 22, 255})
#define EXPLOSION_COLOR WHITE
#define TEXT_COLOR ((Color){135, 206, 250, 255})
#define FLICKERING_ALPHA 150

#define SHIP_SPRINT (.0005f * GetFrameTime() * MULTIPLIER * WINDOW_WIDTH / WSCALE)
#define SHIP_ROTATION_SPEED (1.f * GetFrameTime() * MULTIPLIER)
#define SHIP_MAX_PROJECTILES 30
#define SHIP_MAX_FUEL (FPS * 75)

#define FIRE_TIME 1.f
#define LASER_SPEED (2.f * GetFrameTime() * MULTIPLIER * WINDOW_WIDTH / WSCALE)
#define LASER_ENERGY 50.f
#define LASER_DISPERSION_RATE (.18f * GetFrameTime() * MULTIPLIER)

#define HYPERSPACE_TIME 3.f
#define HYPERSPACE_RELOADING (HYPERSPACE_TIME + 1)
#define HYPERSPACE_READY (HYPERSPACE_RELOADING + 10)

#define PARTICLE_VELOCITY (3.1f * GetFrameTime() * MULTIPLIER * WINDOW_WIDTH / WSCALE)
#define WAIT_TIME 5.f
#define LARGE_EXPLOSION_TIME (WAIT_TIME - .43f)
#define SMALL_EXPLOSION_TIME (WAIT_TIME - .23f)
#define LARGE_EXPLOSION_PARTICLES 100
#define SMALL_EXPLOSION_PARTICLES 20

// Universal gravitational constant
#define G 6.67428e-11

#define DEG_TO_RAD ((PI) / 180)

#endif //__XSPACEWAR_HPP__