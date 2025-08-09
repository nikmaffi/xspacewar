#include <Background.hpp>

Background::Background(const char *textureRes):
flickeringTimer(.0f),
texture(LoadTexture(textureRes)),
color(WHITE) {
	SetTextureWrap(texture, TEXTURE_WRAP_REPEAT);
}

Background::~Background() {
	UnloadTexture(texture);
}

void Background::draw(void) {
	flickeringTimer += GetFrameTime() * flickeringMonitorEffect;

	if(flickeringTimer >= FLICKER_FRAME_INTERVAL) {
		flickeringTimer = .0f;
		
		if(color.a == 185) {
			color.a = 255;
		} else {
			color.a = 185;
		}
	}

	DrawTextureRec(texture, {0.f, 0.f, WINDOW_WIDTH, WINDOW_HEIGHT}, {0.f, 0.f}, color);
}