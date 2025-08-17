#include <Interface.hpp>

Interface::Interface(const Vector2 &pos, const Font &font, const Color &color):
pos(pos),
text(""),
font(font),
color(color),
flickeringTimer(.0f) {
}

void Interface::update(const char *str, ...) {
    va_list args;
    text = "";

    // Initializing variadic function arguments
    va_start(args, str);

    // Parsing the string 
    for(const char *k = str; *k != '\0'; k++) {
        // Non-reserved symbols
        if(*k != '$') {
            text += *k;
            continue;
        }

        k++;

        // Reserved symbols (flags, players number)
        if(*k == '$') {
            text += '$';
        } else if(*k == 'f') {
            text += (va_arg(args, int) ? "[X]" : "[ ]");
        } else if(*k == 'p') {
            text += "[" + std::to_string(va_arg(args, int)) += "]";
        }
    }

    // Closing variadic functions parameters
    va_end(args);
}

void Interface::draw(void) {
    // Updating flickering effect timer
    flickeringTimer += GetFrameTime();

    // Resetting the flickering timer
    if(flickeringTimer >= FLICKERING_INTERVAL) {
        flickeringTimer = .0f;

        // Simulating flickering effect
        // Adjusting texture alpha value based on previous one
        if(color.a != 255) {
            color.a = 255;
        } else {
            color.a = __flickeringEffectValue;
        }
    }

    // Computing text dimensions
    Vector2 dim = MeasureTextEx(font, text.c_str(), font.baseSize, 0.f);
    // Drawing the interface text in screen center
    DrawTextEx(font, text.c_str(), {pos.x - dim.x / 2, pos.y - dim.y / 2}, font.baseSize, 0.f, color);
}
