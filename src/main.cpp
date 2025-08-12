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

int main(int argc, char *argv[]) {
    if(argc != 2) {
        gamePath = ".";
    } else if(argc == 2) {
        gamePath = argv[1];
    }

    initWindow();
    InitAudioDevice();

    GameEngine engine;
    engine.gameLoop();

    return EXIT_SUCCESS;
}