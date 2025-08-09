#ifndef __CRTMONITOR_HPP__
#define __CRTMONITOR_HPP__

#include <xspacewar.hpp>

class CRTMonitor {
private:
	bool running;
	Texture texture;
public:
	CRTMonitor(const char *textureRes);
	~CRTMonitor();
	bool isRunning(void) const;
	void powerOn(void);
	void powerOff(void);
	void draw(void);
};

#endif //__CRTMONITOR_HPP__