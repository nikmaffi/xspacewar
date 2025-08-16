#include <GameEngine.hpp>

void initWindow(void) {
    int w, h;

    // Starting a small window to obtain monitor width and height
    InitWindow(1, 1, "");
    w = WINDOW_WIDTH;
    h = WINDOW_HEIGHT;
    CloseWindow();

    // Setting fullscreen flag
    SetConfigFlags(FLAG_FULLSCREEN_MODE);
    // Initializing real window with title
    InitWindow(w, h, "XSpacewar!");
}

int main(int argc, char *argv[]) {
    // Set game path to argument if provided, otherwise use current directory
    gamePath = (argc == 2) ? argv[1] : ".";

    // Initializing subsystems
    initWindow();
    InitAudioDevice();

    // Starting game loop
    GameEngine engine;
    engine.gameLoop();

    return EXIT_SUCCESS;
}
