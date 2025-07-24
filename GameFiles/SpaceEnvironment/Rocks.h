#pragma once
#include "Physics-Colors/Randomize.h"
#include "Physics-Colors/Colors.h"
#include "SpaceEnvironment/Entity.h"
#include <vector>
#include <cstdint>

class Pellets;
class Ship;

enum class RockState {
    STATIC,
    DETONATING,
    DESTROYED
};

class Rocks {
private:
    std::vector<Entity*> rocks;
    std::vector<RockState> rockStates;
    std::vector<float> detonationTimers;

    Randomize* randomize;
    Colors* colors;

    unsigned int maxRocks;
    float rockTimer;
    float rockTimerMax;
    float baseVelocity;

    sf::Vector2u textureSize;

    float rockAnimationSpeed;
    float detonationDuration;

    void initVariables();
    void initRandomize();
    void initColors();

    void spawnRock(sf::Vector2f pos, sf::Vector2f window_size);
    void deleteRock(unsigned int index);

    sf::Vector2f calculateFrameCoords(int frameIndex);
    int getMaxFramesForTexture();

    void triggerRockDetonation(unsigned int index);
    void updateRockAnimation(unsigned int index, float dt);

public:
    Rocks();
    ~Rocks();

    sf::FloatRect getRockBounds(Entity* rock);
    void reserveCapacity(size_t capacity);

    void update(float dt, sf::Vector2f window_size, Ship& ship, Pellets& pellets, std::uint16_t& score, int& health);
    void render(sf::RenderWindow& window);

    void setAnimationSpeed(float speed) { rockAnimationSpeed = speed; }
    float getAnimationSpeed() const { return rockAnimationSpeed; }
    void setDetonationDuration(float duration) { detonationDuration = duration; }

    size_t getRockCount() const { return rocks.size(); }
    void clearAllRocks();
};
