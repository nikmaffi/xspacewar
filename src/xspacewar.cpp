#include <xspacewar.hpp>

// Global parameters
unsigned char __burnInEffectValue = 255;
unsigned char __flickeringEffectValue = 150;
float         __volumeValue = 0.f;
bool          shipProjectilesLimit = true;
bool          shipFuelLimit = true;
bool          blackHoleAsAnomaly = false;
bool          retroStyleShips = true;
bool          oneShotOneKill = false;
size_t        numPlayers = 2;

// Argv path used for locate game resources
std::string   gamePath = ".";

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
