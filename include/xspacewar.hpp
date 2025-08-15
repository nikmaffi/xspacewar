#ifndef __XSPACEWAR_HPP__
#define __XSPACEWAR_HPP__

#include <raylib.h>

#include <vector>
#include <cmath>
#include <random>
#include <fstream>

// ########################################################################################################################################
// GENERICS
// ########################################################################################################################################

// Parameters
#define NUM_ACTIONS 5
#define MAX_PLAYERS 4
#define MIN_PLAYERS 2

// Math
#define G 6.67428e-11
#define EPSILON 1e-3



// ########################################################################################################################################
// GLOBALS
// ########################################################################################################################################

// Parameters
extern bool   burnInMonitorEffect;
extern bool   flickeringMonitorEffect;
extern bool   shipProjectilesLimit;
extern bool   shipFuelLimit;
extern bool   blackHoleAsAnomaly;
extern bool   retroStyleShips;
extern bool   oneShotOneKill;
extern bool   playSounds;
extern size_t numPlayers;

// Argv path used for locate game resources
extern std::string gamePath;

// Keyboard map [turbo, shoot, hyperspace, left, right]
const KeyboardKey keyboardMap[MAX_PLAYERS][NUM_ACTIONS] = {
	{KEY_S, KEY_W, KEY_E, KEY_A, KEY_D},
	{KEY_DOWN, KEY_UP, KEY_RIGHT_CONTROL, KEY_LEFT, KEY_RIGHT},
	{KEY_G, KEY_T, KEY_Y, KEY_F, KEY_H},
	{KEY_K, KEY_I, KEY_O, KEY_J, KEY_L},
};



// ########################################################################################################################################
// GRAPHICS
// ########################################################################################################################################

// Game window
#define WINDOW_WIDTH ((float)GetMonitorWidth(0))
#define WINDOW_HEIGHT ((float)GetMonitorHeight(0))
#define WINDOW_CENTER_X (WINDOW_WIDTH / 2.f)
#define WINDOW_CENTER_Y (WINDOW_HEIGHT / 2.f)
#define WSCALE 1920.f
#define HSCALE 1080.f

// Parameters
#define FPS 30
#define DT_MULTIPLIER 60.f
#define PLAYER_P_DT_MULTIPLIER 15.f
#define FLICKERING_INTERVAL .08f
#define FLICKERING_ALPHA 150



// ########################################################################################################################################
// EXPLOSIONS
// ########################################################################################################################################

// Parameters
#define EXPLOSION_COLOR WHITE
#define EXPLOSION_PARTICLE_VELOCITY (3.1f * GetFrameTime() * DT_MULTIPLIER * WINDOW_WIDTH / WSCALE)
#define EXPLOSION_WAIT_TIME 5.f
#define EXPLOSION_LARGE_TIME (EXPLOSION_WAIT_TIME - .43f)
#define EXPLOSION_SMALL_TIME (EXPLOSION_WAIT_TIME - .23f)
#define EXPLOSION_LARGE_PARTICLES 100
#define EXPLOSION_SMALL_PARTICLES 20



// ########################################################################################################################################
// PHOSPHORUS
// ########################################################################################################################################

// Parameters
#define PHOSPHORUS_BIG_TIMER 255.f
#define PHOSPHORUS_SMALL_TIMER 100.f
#define PHOSPHORUS_COLOR(alpha) ((Color){139, 180, 60, (unsigned char)alpha})



// ########################################################################################################################################
// VIDEO TERMINAL
// ########################################################################################################################################

// Basic structure
#define VT_POS ((Vector2){WINDOW_CENTER_X, WINDOW_CENTER_Y})
#define VT_CIRCLES_SEGS 64
#define VT_MONITOR_RADIUS (WINDOW_HEIGHT / 2.f - 72 * WINDOW_WIDTH / WSCALE)
#define VT_RING_RADIUS (VT_MONITOR_RADIUS + 25.f)
#define VT_MONITOR_COLOR ((Color){34, 42, 22, 255})
#define VT_RING_COLOR ((Color){198, 211, 216, 255})
#define VT_CASE_COLOR ((Color){108, 122, 152, 255})

// Trademark
#define VT_TRADEMARK " NKM\nT7000"
#define VT_TRADEMARK_POS(dim) ((Vector2){WINDOW_WIDTH - 350 - ((Vector2)dim).x / 2, WINDOW_HEIGHT - 150 - ((Vector2)dim).y / 2})
#define VT_TRADEMARK_FONT_SIZE 75
#define VT_TRADEMARK_COLOR ((Color){56, 57, 59, 255})



// ########################################################################################################################################
// PLAYER
// ########################################################################################################################################

// Parameters
#define PLAYER_SPRINT (.0005f * GetFrameTime() * DT_MULTIPLIER * WINDOW_WIDTH / WSCALE)
#define PLAYER_ROTATION_SPEED (1.5f * GetFrameTime() * DT_MULTIPLIER)
#define PLAYER_MAX_PROJECTILES 30
#define PLAYER_MAX_FUEL (FPS * 75)

// Timers
#define PLAYER_FIRE_TIME 1.f
#define PLAYER_HYPERSPACE_TIME 3.f
#define PLAYER_HYPERSPACE_RELOADING (PLAYER_HYPERSPACE_TIME + 1)
#define PLAYER_HYPERSPACE_READY (PLAYER_HYPERSPACE_RELOADING + 10)

// Start positions
#define PLAYER_1_START_POS ((Vector2){WINDOW_CENTER_X - 290.f * WINDOW_WIDTH / WSCALE, WINDOW_CENTER_Y + 290.f * WINDOW_HEIGHT / HSCALE})
#define PLAYER_2_START_POS ((Vector2){WINDOW_CENTER_X + 290.f * WINDOW_WIDTH / WSCALE, WINDOW_CENTER_Y - 290.f * WINDOW_HEIGHT / HSCALE})
#define PLAYER_3_START_POS ((Vector2){WINDOW_CENTER_X - 290.f * WINDOW_WIDTH / WSCALE, WINDOW_CENTER_Y - 290.f * WINDOW_HEIGHT / HSCALE})
#define PLAYER_4_START_POS ((Vector2){WINDOW_CENTER_X + 290.f * WINDOW_WIDTH / WSCALE, WINDOW_CENTER_Y + 290.f * WINDOW_HEIGHT / HSCALE})

// Start angles
#define PLAYER_1_START_ANGLE 270.f
#define PLAYER_2_START_ANGLE 90.f 
#define PLAYER_3_START_ANGLE 90.f 
#define PLAYER_4_START_ANGLE 270.f

// Attributes
#define PLAYER_TEX_SCALE (.8f * WINDOW_WIDTH / WSCALE)



// ########################################################################################################################################
// LASER
// ########################################################################################################################################

// Parameters
#define LASER_SPEED (2.f * GetFrameTime() * DT_MULTIPLIER * WINDOW_WIDTH / WSCALE)
#define LASER_ENERGY 50.f
#define LASER_DISPERSION_RATE (.18f * GetFrameTime() * DT_MULTIPLIER)

// Attributes
#define LASER_TEX_SCALE (.5f * WINDOW_WIDTH / WSCALE)



// ########################################################################################################################################
// USER INTERFACE
// ########################################################################################################################################

// Attributes
#define UI_POS ((Vector2){WINDOW_CENTER_X, WINDOW_CENTER_Y})
#define UI_FONT_SIZE (25 * WINDOW_WIDTH / WSCALE)
#define UI_TEXT_COLOR ((Color){135, 206, 250, 255})



// ########################################################################################################################################
// ANOMALY
// ########################################################################################################################################

// Attributes
#define ANOMALY_MASS 1e12
#define ANOMALY_POS ((Vector2){WINDOW_CENTER_X, WINDOW_CENTER_Y})

#endif //__XSPACEWAR_HPP__