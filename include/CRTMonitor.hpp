#ifndef __CRTMONITOR_HPP__
#define __CRTMONITOR_HPP__

#include <xspacewar.hpp>

class CRTMonitor {
private:
	bool running;
	Font font;
public:
	CRTMonitor(const char *fontRes);
	~CRTMonitor();
	bool isRunning(void) const;
	void powerOn(void);
	void powerOff(void);
	void draw(void);
};

#endif //__CRTMONITOR_HPP__