#include <CRTMonitor.hpp>

CRTMonitor::CRTMonitor(const char *textureRes) :
running(false),
texture(LoadTexture(textureRes)) {
}

CRTMonitor::~CRTMonitor() {
    UnloadTexture(texture);
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
	float newWidth = texture.width * WINDOW_HEIGHT / texture.height;
    float newHeight = texture.height * WINDOW_HEIGHT / texture.height;

    DrawRing(
        {WINDOW_CENTER_X, WINDOW_CENTER_Y},
        MONITOR_RADIUS, WINDOW_WIDTH, .0f, 360.f, 36.f, BACKGROUND_COLOR
    );

    DrawTexturePro(
        texture,
        (Rectangle){0.f, 0.f, (float)texture.width, (float)texture.height},
        (Rectangle){(WINDOW_WIDTH - newWidth) / 2, (WINDOW_HEIGHT - newHeight) / 2, newWidth, newHeight},
        (Vector2){0.f, 0.f},
        0.f,
        WHITE
    );
}