#ifndef __BACKGROUND_HPP__
#define __BACKGROUND_HPP__

#include <xspacewar.hpp>

class Background {
private:
	float flickeringTimer;

	Texture texture;
    Color color;
public:
	Background(const char *textureRes);
	~Background();
	virtual void draw(void);
};

#endif //__BACKGROUND_HPP__