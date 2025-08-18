#include <CRTMonitor.hpp>

CRTMonitor::CRTMonitor(const Vector2 &pos, float inRadius, float outRadius, const Vector2 &tmPos, const Texture &tmTex, float tmScale) :
running(false),
pos(pos),
inRadius(inRadius),
outRadius(outRadius),
tmPos(tmPos),
tmTex(tmTex),
tmScale(tmScale) {
}

bool CRTMonitor::isRunning(void) const {
	return running;
}

void CRTMonitor::powerOn(void) {
	running = true;
}

void CRTMonitor::powerOff(void) {
	running = false;
}

void CRTMonitor::draw(void) {
    // Drawing monitor case
    DrawRing(pos, inRadius, WINDOW_WIDTH, .0f, 360.f, VT_CIRCLES_SEGS, VT_CASE_COLOR);

    // Drawing monitor silver ring around the screen
    DrawRing(pos, inRadius, outRadius, .0f, 360.f, VT_CIRCLES_SEGS, VT_RING_COLOR);

    // Drawing system trademark
    DrawTexturePro(
        tmTex,
        {0, 0, (float)tmTex.width, (float)tmTex.height},
        {tmPos.x, tmPos.y, tmTex.width * tmScale, tmTex.height * tmScale},
        {tmTex.width * tmScale / 2, tmTex.height * tmScale / 2},
        0,
        WHITE
    );
}
