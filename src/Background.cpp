#include <Background.hpp>

Background::Background(const Texture &texture):
texture(texture),
color(WHITE) {
	// Setting texture repeat flag
	SetTextureWrap(texture, TEXTURE_WRAP_REPEAT);
}

void Background::draw(void) {
	// Resetting the flickering timer
	if(__flickeringTimer >= FLICKERING_INTERVAL) {
		// Simulating flickering effect
		// Adjusting texture alpha value based on previous one
		if(color.a != 255) {
			color.a = 255;
		} else {
			color.a = __flickeringEffectValue;
		}
	}

	// Repeating texture drawing on the entire window
	DrawTextureRec(texture, {0.f, 0.f, WINDOW_WIDTH, WINDOW_HEIGHT}, {0.f, 0.f}, color);
}
