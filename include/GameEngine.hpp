#ifndef __GAMEENGINE_HPP__
#define __GAMEENGINE_HPP__

#include <xspacewar.hpp>
#include <CRTMonitor.hpp>
#include <Phosphorus.hpp>
#include <Background.hpp>
#include <Anomaly.hpp>
#include <Player.hpp>
#include <Interface.hpp>

class GameEngine {
private:
	unsigned p1jc, p2jc, p1jid, p2jid;

	CRTMonitor monitor;
	Interface userInterface;

	Background background;
	Player player1, player2;
	Anomaly anomaly;
	Phosphorus phosphorus;

	void loadData(void);

	void eventsHandler(void);
	void update(void);
	void draw(void);
public:
	GameEngine(void);
	~GameEngine();
	void gameLoop(void);
};

#endif //__GAMEENGINE_HPP__