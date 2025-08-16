#include <CRTMonitor.hpp>

CRTMonitor::CRTMonitor(const char *fontRes) :
running(false),
font(LoadFontEx(fontRes, VT_TRADEMARK_FONT_SIZE, 0, 0)),
trademarkSize(MeasureTextEx(font, VT_TRADEMARK, VT_TRADEMARK_FONT_SIZE, 0.f)) {
}

CRTMonitor::~CRTMonitor() {
    UnloadFont(font);
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
    DrawRing(VT_POS, VT_MONITOR_RADIUS, WINDOW_WIDTH, .0f, 360.f, VT_CIRCLES_SEGS, VT_CASE_COLOR);

    // Drawing monitor silver ring around the screen
    DrawRing(VT_POS, VT_MONITOR_RADIUS, VT_RING_RADIUS, .0f, 360.f, VT_CIRCLES_SEGS, VT_RING_COLOR);

    // Drawing system trademark
    DrawTextEx(font, VT_TRADEMARK, VT_TRADEMARK_POS(trademarkSize), VT_TRADEMARK_FONT_SIZE, 0.f, VT_TRADEMARK_COLOR);
}
