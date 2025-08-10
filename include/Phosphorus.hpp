#ifndef __PHOSPHORUS_HPP__
#define __PHOSPHORUS_HPP__

#include <xspacewar.hpp>
#include <Player.hpp>
#include <Laser.hpp>
#include <Particles.hpp>
#include <Anomaly.hpp>
#include <Interface.hpp>

struct GhostEntity {
    Vector2 pos;
	float angle;
	Texture texture;
	Rectangle sprite;
    Color color;
	float scale;
    float timer;

    GhostEntity(const Player &player);
    GhostEntity(const Laser &laser);
    GhostEntity(const Anomaly &anomaly);
};

struct GhostParticles {
    std::vector<Vector2> pos;
    Color color;
    float timer;

    GhostParticles(const Particles &particles, size_t size);
};

struct GhostInterface {
    Vector2 pos;
	Color color;
    std::string text;
	Font font;
    unsigned int fontSize;
    float timer;

    GhostInterface(const Interface &interface);
};

class Phosphorus {
private:
    const Player *players;
    const Anomaly &anomaly;
    const Interface &interface;

    std::vector<GhostEntity> entitiesPhosphorus;
    std::vector<GhostInterface> interfacePhosphorus;
    std::vector<GhostParticles> playerExplosionPhosphorus;
    std::vector<std::vector<GhostEntity>> playerLaserPhosphorus;
    std::vector<std::vector<GhostParticles>> playerLaserExplosionPhosphorus;

    void updatePlayer(const Player &player, bool monitorRunning);
    void updatePlayerExplosion(const Player &player, bool monitorRunning);
    void updatePlayerLaser(const Player &player, bool monitorRunning);
    void updatePlayerLaserExplosion(const Player &player, bool monitorRunning);
public:
    Phosphorus(const Player *players, const Anomaly &anomaly, const Interface &interface);
    void update(bool monitorRunning);
    void draw(bool monitorRunning);
};

#endif //__PHOSPHORUS_HPP__