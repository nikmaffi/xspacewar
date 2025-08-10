#ifndef __GAMEENGINE_HPP__
#define __GAMEENGINE_HPP__

#include <xspacewar.hpp>
#include <CRTMonitor.hpp>
#include <Phosphorus.hpp>
#include <Background.hpp>
#include <Anomaly.hpp>
#include <Player.hpp>
#include <Interface.hpp>
#include <JoystickHandler.hpp>

class GameEngine {
private:
	Image icon;

	unsigned joystickConfigs[MAX_PLAYERS];
	JoystickHandler joystickHandler;

	Sound laserSound;
	Sound explosionSound;

	CRTMonitor monitor;
	Interface userInterface;

	Background background;
	Player players[MAX_PLAYERS];
	Anomaly anomaly;
	Phosphorus phosphorus;

	void loadData(void);
	void saveData(void);

	void eventsHandler(void);
	void update(void);
	void draw(void);
public:
	GameEngine(void);
	~GameEngine();
	void gameLoop(void);
};

#endif //__GAMEENGINE_HPP__