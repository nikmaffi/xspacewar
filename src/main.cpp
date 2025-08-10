#include <GameEngine.hpp>

void initWindow(void) {
    int w, h;

    InitWindow(1, 1, "");
    w = WINDOW_WIDTH;
    h = WINDOW_HEIGHT;
    CloseWindow();

    SetConfigFlags(FLAG_FULLSCREEN_MODE);
    InitWindow(w, h, "XSpacewar!");
}

int main(void) {
    initWindow();
    InitAudioDevice();

    GameEngine engine;
    engine.gameLoop();

    return EXIT_SUCCESS;
}