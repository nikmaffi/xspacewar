#ifndef __KNOB_HPP__
#define __KNOB_HPP__

#include <xspacewar.hpp>

class Knob {
private:
    // Physics attributes
    Vector2 pos;
    float angle;

    // Game objects atributes
    Texture texture;
    Texture valueTex;
    float scale;

    // Label attributes
    Font font;
    std::string label;
    Vector2 labelDim;
public:
    Knob(const Vector2 &pos, const Texture &texture, const Texture &valueTex, float scale, const char *label, const Font &font);
    void setAngle(float angle);
    void update(const Vector2 &mousePos, void (*updateValue)(float));
    void draw(void);
};

#endif //__KNOB_HPP__
