#ifndef __CRTMONITOR_HPP__
#define __CRTMONITOR_HPP__

#include <xspacewar.hpp>

class CRTMonitor {
private:
	// Misc attributes
	bool running;

	// Trademark attributes
	Vector2 tmPos;
	Texture tmTex;
	float tmScale;
public:
	CRTMonitor(const Vector2 &tmPos, const Texture &tmTex, float tmScale);
	bool isRunning(void) const;
	void powerOn(void);
	void powerOff(void);
	void draw(void);
};

#endif //__CRTMONITOR_HPP__
