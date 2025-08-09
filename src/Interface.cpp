#include <Interface.hpp>

Interface::Interface(const Vector2 &pos, const char *fontRes, unsigned int size, const Color &color):
pos(pos),
color(color),
text(""),
font(LoadFont(fontRes)),
fontSize(size),
flickeringTimer(.0f) {
}

Interface::~Interface() {
    UnloadFont(font);
}

void Interface::update(const char *str, ...) {
    va_list args;
    text = "";

    va_start(args, str);

    for(const char *k = str; *k != '\0'; k++) {
        if(*k != '$') {
            text += *k;
            continue;
        }

        k++;

        if(*k == '$') {
            text += '$';
        } else if(*k == 'f') {
            text += (va_arg(args, int) ? "[X]" : "[ ]");
        } else if(*k == 'j') {
            text += joystickConfigsNames[va_arg(args, int)];
        }
    }

    va_end(args);

    if(!flickeringMonitorEffect) {
        color.a = 255;
    }
}

void Interface::draw(void) {
    flickeringTimer += GetFrameTime() * flickeringMonitorEffect;

    if(flickeringTimer >= FLICKER_FRAME_INTERVAL) {
        flickeringTimer = .0f;

        if(color.a == 185) {
            color.a = 255;
        } else {
            color.a = 185;
        }
    }

    Vector2 dim = MeasureTextEx(font, text.c_str(), fontSize, 0.f);
    DrawTextEx(font, text.c_str(), {pos.x - dim.x / 2, pos.y - dim.y / 2}, fontSize, 0.f, color);
}