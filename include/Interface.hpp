#ifndef __INTERFACE_HPP__
#define __INTERFACE_HPP__

#include <xspacewar.hpp>

struct GhostInterface;

class Interface {
	friend struct GhostInterface;

private:
	// Game object attributes
	Vector2 pos;
    std::string text;
	Font font;
	Color color;

	// Timers
	float flickeringTimer;
public:
	Interface(const Vector2 &pos, const Font &font, const Color &color);
	Interface(void);
	void update(const char *str, ...);
	void draw(void);
};

#endif //__INTERFACE_HPP__
