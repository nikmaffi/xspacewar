#include <Background.hpp>

Background::Background(const Texture &texture):
texture(texture),
color(WHITE),
flickeringTimer(.0f) {
	// Setting texture repeat flag
	SetTextureWrap(texture, TEXTURE_WRAP_REPEAT);
}

void Background::draw(void) {
	// Restoring alpha value if flickering effect is not active
    if(!flickeringMonitorEffect) {
        color.a = 255;
    } else {
		// Updating flickering effect timer
		flickeringTimer += GetFrameTime();

		// Resetting the flickering timer
		if(flickeringTimer >= FLICKERING_INTERVAL) {
			flickeringTimer = .0f;

			// Simulating flickering effect
			// Adjusting texture alpha value based on previous one
			if(color.a == FLICKERING_ALPHA) {
				color.a = 255;
			} else {
				color.a = FLICKERING_ALPHA;
			}
		}
	}

	// Repeating texture drawing on the entire window
	DrawTextureRec(texture, {0.f, 0.f, WINDOW_WIDTH, WINDOW_HEIGHT}, {0.f, 0.f}, color);
}
