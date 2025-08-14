#include <CRTMonitor.hpp>

CRTMonitor::CRTMonitor(const char *fontRes) :
running(false),
font(LoadFontEx(fontRes, 75, 0, 0)) {
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
	//float newWidth = texture.width * WINDOW_HEIGHT / texture.height;
    //float newHeight = texture.height * WINDOW_HEIGHT / texture.height;

    DrawRing(
        {WINDOW_CENTER_X, WINDOW_CENTER_Y},
        MONITOR_RADIUS, WINDOW_WIDTH, .0f, 360.f, 36.f, (Color){108, 122, 152, 255}//BACKGROUND_COLOR
    );

    DrawRing(
        {WINDOW_CENTER_X, WINDOW_CENTER_Y},
        MONITOR_RADIUS, MONITOR_RADIUS + 22.f, .0f, 360.f, 64.f, (Color){198, 211, 216, 255}
    );

    Vector2 dim = MeasureTextEx(font, " NKM\nT7000", 75, 0.f);
    DrawTextEx(
        font, " NKM\nT7000", {WINDOW_WIDTH - 350 - dim.x / 2, WINDOW_HEIGHT - 150 - dim.y / 2},
        75, 0.f, (Color){56, 57, 59, 255}
    );

    /*DrawTexturePro(
        texture,
        (Rectangle){0.f, 0.f, (float)texture.width, (float)texture.height},
        (Rectangle){(WINDOW_WIDTH - newWidth) / 2, (WINDOW_HEIGHT - newHeight) / 2, newWidth, newHeight},
        (Vector2){0.f, 0.f},
        0.f,
        WHITE
    );*/
}