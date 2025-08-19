#include <xspacewar.hpp>

// Global values
unsigned char __burnInEffectValue = 255;
unsigned char __flickeringEffectValue = 150;
float __volumeValue = 0.f;
size_t __numPlayers = 2;

// Global timers
float __flickeringTimer = .0f;

// Global flags
bool __shipProjectilesLimit = true;
bool __shipFuelLimit = true;
bool __blackHoleAsAnomaly = false;
bool __retroStyleShips = true;
bool __oneShotOneKill = false;

// Argv path used for locate game resources
std::string __gamePath = ".";

// Knobs update functions
void updateFlickeringEffectValue(float angle) {
    __flickeringEffectValue = 255 + (angle / 180) * (FLICKERING_MIN_ALPHA - 255);
}

void updateBurnInEffectValue(float angle) {
    __burnInEffectValue = angle / 180 * 255;
}

void updateVolumeValue(float angle) {
    __volumeValue = angle / 180;
}
