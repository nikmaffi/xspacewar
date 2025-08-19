#include <GameEngine.hpp>

void initWindow(void) {
    // Starting a small window to obtain monitor width and height
    InitWindow(1, 1, "");
    WINDOW_WIDTH = (float)GetMonitorWidth(0);
    WINDOW_HEIGHT = (float)GetMonitorHeight(0);
    CloseWindow();

    // Setting fullscreen flag
    SetConfigFlags(FLAG_FULLSCREEN_MODE);
    // Initializing real window with title
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "XSpacewar!");
}

int main(int argc, char *argv[]) {
    // Set game path to argument if provided, otherwise use current directory
    __gamePath = (argc == 2) ? argv[1] : ".";

    // Initializing subsystems
    initWindow();
    InitAudioDevice();

    // Starting game loop
    GameEngine engine;
    engine.gameLoop();

    return EXIT_SUCCESS;
}
