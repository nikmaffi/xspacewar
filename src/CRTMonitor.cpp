#include <CRTMonitor.hpp>

CRTMonitor::CRTMonitor(const Vector2 &pos, const Texture &texture, const Vector2 &tmPos, const Texture &tmTex, float tmScale) :
running(false),
pos(pos),
texture(texture),
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
    // Drawing monitor texture
    DrawTexturePro(
        texture,
        {0, 0, (float)texture.width, (float)texture.height},
        {pos.x, pos.y, WINDOW_WIDTH, WINDOW_HEIGHT},
        {WINDOW_CENTER_X, WINDOW_CENTER_Y},
        0,
        WHITE
    );

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
