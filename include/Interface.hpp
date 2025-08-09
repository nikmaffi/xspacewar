#ifndef __INTERFACE_HPP__
#define __INTERFACE_HPP__

#include <xspacewar.hpp>

#include <string>

class GhostInterface;

class Interface {
private:
    Vector2 pos;

	Color color;
    std::string text;
	Font font;
    unsigned int fontSize;

	float flickeringTimer;
public:
	Interface(const Vector2 &pos, const char *fontRes, unsigned int size, const Color &color);
	~Interface();
	Interface(void);
	void update(const char *str, ...);
	void draw(void);

	friend class GhostInterface;
};

#endif //__INTERFACE_HPP__