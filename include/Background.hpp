#ifndef __BACKGROUND_HPP__
#define __BACKGROUND_HPP__

#include <xspacewar.hpp>

class Background {
private:
	// Game object attributes
	Texture texture;
    Color color;

	// Timers
	float flickeringTimer;
public:
	Background(const Texture &texture);
	void draw(void);
};

#endif //__BACKGROUND_HPP__
