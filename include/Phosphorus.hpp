#ifndef __PHOSPHORUS_HPP__
#define __PHOSPHORUS_HPP__

#include <xspacewar.hpp>
#include <Player.hpp>
#include <Laser.hpp>
#include <Particles.hpp>
#include <Anomaly.hpp>
#include <Interface.hpp>

// Used for players, lasers and anomaly
struct GhostEntity {
    // Physics attributes
    Vector2 pos;
	float angle;

    // Game object attributes
	Texture texture;
	Rectangle sprite;
    Color color;
	float scale;

    // Timers
    float timer;

    GhostEntity(const Player &player);
    GhostEntity(const Laser &laser);
    GhostEntity(const Anomaly &anomaly);
};

// Used for players and lasers explosions
struct GhostParticles {
    // Physics attributes
    std::vector<Vector2> pos;

    // Game object attributes
    Color color;

    // Timers
    float timer;

    GhostParticles(const Particles &particles, size_t size);
};

// Used for the interface text
struct GhostInterface {
    // Game object attributes
    Vector2 pos;
    std::string text;
	Font font;
	Color color;

    // Timers
    float timer;

    GhostInterface(const Interface &interface);
};

class Phosphorus {
private:
    // Game objects references
    const Player *players;
    const Anomaly &anomaly;
    const Interface &interface;

    // Phosphorus effect textures vectors
    std::vector<GhostEntity> entitiesPhosphorus;
    std::vector<GhostInterface> interfacePhosphorus;
    std::vector<GhostParticles> playerExplosionPhosphorus;
    std::vector<std::vector<GhostEntity>> playerLaserPhosphorus;
    std::vector<std::vector<GhostParticles>> playerLaserExplosionPhosphorus;

    // Phosphorus effect updates
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
