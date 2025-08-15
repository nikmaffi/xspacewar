#include <Anomaly.hpp>

Anomaly::Anomaly(const Vector2 &pos, const char *textureRes, float scale):
pos(pos),
mass(ANOMALY_MASS),
angle(.0f),
texture(LoadTexture(textureRes)),
sprite((Rectangle){.0f, .0f, (float)texture.width, texture.height / 2.f}),
color(WHITE),
scale(scale),
flickeringTimer(.0f),
dist(0.f, 359.f) {
}

Anomaly::~Anomaly() {
	UnloadTexture(texture);
}

const Vector2 &Anomaly::getPosition(void) const {
	return pos;
}

Circle Anomaly::getCircle(void) const {
	return Circle(pos, 2);
}

void Anomaly::changeAnomaly(void) {
	if(blackHoleAsAnomaly) {
        sprite = (Rectangle){.0f, texture.height / 2.f, (float)texture.width, texture.height / 2.f};
	} else {
        sprite = (Rectangle){.0f, .0f, (float)texture.width, texture.height / 2.f};
	}
}

Vector2 Anomaly::attract(const Vector2 &pos) const {
	Vector2 dist = (Vector2){this->pos.x - pos.x, this->pos.y - pos.y};
	float distMag = std::sqrt(dist.x * dist.x + dist.y * dist.y) * WSCALE / WINDOW_WIDTH + EPSILON;
	float forceMag = G * this->mass / (distMag * distMag * distMag);

	return (Vector2){
        (forceMag * dist.x) * GetFrameTime() * DT_MULTIPLIER,
        (forceMag * dist.y) * GetFrameTime() * DT_MULTIPLIER
    };
}

void Anomaly::draw(void) {
	flickeringTimer += GetFrameTime() * flickeringMonitorEffect;

	if(flickeringTimer >= FLICKERING_INTERVAL) {
		flickeringTimer = .0f;

		if(color.a == FLICKERING_ALPHA) {
			color.a = 255;
		} else {
			color.a = FLICKERING_ALPHA;
		}
	}

	angle = dist(rd);

    DrawTexturePro(
        texture,
        sprite,
        (Rectangle){pos.x, pos.y, sprite.width * scale, sprite.height * scale},
        (Vector2){sprite.width * scale / 2.f, sprite.height * scale / 2.f},
        angle,
        color
    );
}