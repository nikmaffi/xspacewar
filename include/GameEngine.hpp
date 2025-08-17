#ifndef __GAMEENGINE_HPP__
#define __GAMEENGINE_HPP__

#include <xspacewar.hpp>
#include <CRTMonitor.hpp>
#include <Knob.hpp>
#include <Interface.hpp>
#include <Background.hpp>
#include <Anomaly.hpp>
#include <Player.hpp>
#include <Phosphorus.hpp>

class GameEngine {
private:
	// Assets - textures
	Image icon;
	Texture knobTex;
	Texture backgroundTex;
	Texture anomalyTex;
	Texture playersTex[MAX_PLAYERS];
	Texture lasersTex[MAX_PLAYERS];

	// Assets - fonts
	Font interfaceFont;

	// Assets - sounds
	Sound laserSound;
	Sound explosionSound;

	// Game objects
	CRTMonitor monitor;
	Knob flickeringKnob;
	Knob burnInKnob;
	Interface userInterface;
	Background background;
	Anomaly anomaly;
	Player players[MAX_PLAYERS];
	Phosphorus phosphorus;

	// Save/load game settings
	void loadData(void);
	void saveData(void);

	// Game loop actions
	void eventsHandler(void);
	void update(void);
	void draw(void);
public:
	GameEngine(void);
	~GameEngine();
	void gameLoop(void);
};

#endif //__GAMEENGINE_HPP__
