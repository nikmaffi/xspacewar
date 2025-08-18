#ifndef __XSPACEWAR_HPP__
#define __XSPACEWAR_HPP__

#include <raylib.h>

#include <vector>
#include <cmath>
#include <random>
#include <fstream>

// ############################################################################################################################
// GENERICS
// ############################################################################################################################

// Parameters
#define NUM_ACTIONS 5
#define MAX_PLAYERS 4
#define MIN_PLAYERS 2

// Math
#define G 6.67428e-11
#define EPSILON 1e-3



// ############################################################################################################################
// GLOBALS
// ############################################################################################################################

// Global parameters
extern unsigned char __burnInEffectValue;
extern unsigned char __flickeringEffectValue;
extern float 		 __volumeValue;
extern bool          shipProjectilesLimit;
extern bool          shipFuelLimit;
extern bool          blackHoleAsAnomaly;
extern bool          retroStyleShips;
extern bool          oneShotOneKill;
extern size_t        numPlayers;

// Argv path used for locate game resources
extern std::string   gamePath;

// Knobs update functions
void updateFlickeringEffectValue(float angle);
void updateBurnInEffectValue(float angle);
void updateVolumeValue(float angle);

// Keyboard map [turbo, shoot, hyperspace, left, right]
const KeyboardKey keyboardMap[MAX_PLAYERS][NUM_ACTIONS] = {
	{KEY_S, KEY_W, KEY_E, KEY_A, KEY_D},
	{KEY_DOWN, KEY_UP, KEY_RIGHT_SHIFT, KEY_LEFT, KEY_RIGHT},
	{KEY_G, KEY_T, KEY_Y, KEY_F, KEY_H},
	{KEY_K, KEY_I, KEY_O, KEY_J, KEY_L},
};



// ############################################################################################################################
// GRAPHICS
// ############################################################################################################################

// Game window
inline float WINDOW_WIDTH;
inline float WINDOW_HEIGHT;

#define WINDOW_CENTER_X (WINDOW_WIDTH / 2)
#define WINDOW_CENTER_Y (WINDOW_HEIGHT / 2)
#define SCALE_FACTOR ((float)std::fmin(WINDOW_WIDTH / 1920, WINDOW_HEIGHT / 1080))

// Parameters
#define FPS 30
#define DT_MULTIPLIER 60.f
#define FLICKERING_INTERVAL .08f
#define FLICKERING_MIN_ALPHA 0



// ############################################################################################################################
// EXPLOSIONS
// ############################################################################################################################

// Parameters
#define EXPLOSION_COLOR WHITE
#define EXPLOSION_PARTICLE_VELOCITY (3.1f * GetFrameTime() * DT_MULTIPLIER * SCALE_FACTOR)
#define EXPLOSION_WAIT_TIME 5.f
#define EXPLOSION_LARGE_TIME (EXPLOSION_WAIT_TIME - .43f)
#define EXPLOSION_SMALL_TIME (EXPLOSION_WAIT_TIME - .23f)
#define EXPLOSION_LARGE_PARTICLES 150
#define EXPLOSION_SMALL_PARTICLES 20



// ############################################################################################################################
// PHOSPHORUS
// ############################################################################################################################

// Parameters
#define PHOSPHORUS_EFFECT_TRIG 1
#define PHOSPHORUS_DT_MULTIPLIER 11.f
#define PHOSPHORUS_COLOR(alpha) ((Color){139, 180, 60, (unsigned char)alpha})



// ############################################################################################################################
// VIDEO TERMINAL
// ############################################################################################################################

// Basic structure
#define VT_POS ((Vector2){WINDOW_CENTER_X, WINDOW_CENTER_Y})
#define VT_MONITOR_RADIUS (WINDOW_HEIGHT / 2 - 72 * SCALE_FACTOR)
#define VT_MONITOR_COLOR ((Color){34, 42, 22, 255})
//#define VT_RING_COLOR ((Color){198, 211, 216, 255})
//#define VT_CASE_COLOR ((Color){60, 77, 95, 255})

// Knobs
#define KNOB_TEX_SCALE (.18f * SCALE_FACTOR)
#define KNOB_FLCK_POS ((Vector2){230 * SCALE_FACTOR, 200 * SCALE_FACTOR})
#define KNOB_BRIN_POS ((Vector2){230 * SCALE_FACTOR, 420 * SCALE_FACTOR})
#define KNOB_VOLM_POS ((Vector2){230 * SCALE_FACTOR, 640 * SCALE_FACTOR})
#define KNOB_FONT_SIZE (40 * SCALE_FACTOR)
#define KNOB_FONT_COLOR ((Color){36, 36, 38, 255})

// Trademark
#define VT_TRADEMARK_SCALE (.25f * SCALE_FACTOR)
#define VT_TRADEMARK_POS ((Vector2){WINDOW_WIDTH - 270 * SCALE_FACTOR, WINDOW_HEIGHT - 150 * SCALE_FACTOR})



// ############################################################################################################################
// PLAYER
// ############################################################################################################################

// Parameters
#define PLAYER_SPRINT (.0005f * GetFrameTime() * DT_MULTIPLIER * SCALE_FACTOR)
#define PLAYER_ROTATION_SPEED (1.5f * GetFrameTime() * DT_MULTIPLIER)
#define PLAYER_MAX_PROJECTILES 30
#define PLAYER_MAX_FUEL (FPS * 75)

// Timers
#define PLAYER_FIRE_TIME 1.f
#define PLAYER_HYPERSPACE_TIME 3.f
#define PLAYER_HYPERSPACE_RELOADING (PLAYER_HYPERSPACE_TIME + 1)
#define PLAYER_HYPERSPACE_READY (PLAYER_HYPERSPACE_RELOADING + 10)

// Start positions
#define PLAYER_1_START_POS ((Vector2){WINDOW_CENTER_X - 290 * SCALE_FACTOR, WINDOW_CENTER_Y + 290 * SCALE_FACTOR})
#define PLAYER_2_START_POS ((Vector2){WINDOW_CENTER_X + 290 * SCALE_FACTOR, WINDOW_CENTER_Y - 290 * SCALE_FACTOR})
#define PLAYER_3_START_POS ((Vector2){WINDOW_CENTER_X - 290 * SCALE_FACTOR, WINDOW_CENTER_Y - 290 * SCALE_FACTOR})
#define PLAYER_4_START_POS ((Vector2){WINDOW_CENTER_X + 290 * SCALE_FACTOR, WINDOW_CENTER_Y + 290 * SCALE_FACTOR})

// Start angles
#define PLAYER_1_START_ANGLE 270.f
#define PLAYER_2_START_ANGLE 90.f 
#define PLAYER_3_START_ANGLE 90.f 
#define PLAYER_4_START_ANGLE 270.f

// Attributes
#define PLAYER_TEX_SCALE (.8f * SCALE_FACTOR)



// ############################################################################################################################
// LASER
// ############################################################################################################################

// Parameters
#define LASER_SPEED (2 * GetFrameTime() * DT_MULTIPLIER * SCALE_FACTOR)
#define LASER_ENERGY 50.f
#define LASER_DISPERSION_RATE (.18f * GetFrameTime() * DT_MULTIPLIER)

// Attributes
#define LASER_TEX_SCALE (.5f * SCALE_FACTOR)



// ############################################################################################################################
// USER INTERFACE
// ############################################################################################################################

// Attributes
#define UI_POS ((Vector2){WINDOW_CENTER_X, WINDOW_CENTER_Y})
#define UI_FONT_SIZE (25 * SCALE_FACTOR)
#define UI_TEXT_COLOR ((Color){135, 206, 250, 255})



// ############################################################################################################################
// ANOMALY
// ############################################################################################################################

// Attributes
#define ANOMALY_MASS 1e12
#define ANOMALY_POS ((Vector2){WINDOW_CENTER_X, WINDOW_CENTER_Y})
#define ANOMALY_COLLISION_RADIUS (2 * SCALE_FACTOR)
#define ANOMALY_SCALE SCALE_FACTOR

#endif //__XSPACEWAR_HPP__
