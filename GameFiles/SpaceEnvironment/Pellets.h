#pragma once

#include "SpaceEnvironment/Entity.h"
#include "Audio-SoundEffects/SoundEffects.h"

class Pellets {
private:
    static constexpr float pi = 3.14593f;

    std::vector<Entity*> pellets;

    float pelletTimer;
    float pelletTimerMax;
    float baseVelocity;
    unsigned int maxPellets;

    Sound* soundEffects;

    void OutOfBounds(unsigned int index, sf::Vector2f window_size);
    void setPelletSprite(Entity* pellet, int column, int row);
    void StartSounds();

    void clearAllPellets();

public:
    Pellets();
    ~Pellets();

    sf::FloatRect getPelletBounds(Entity* pellet);

    void firePellet(float angle, sf::Vector2f position);
    std::vector<Entity*> &getPellets();

    void update(float dt, sf::Vector2f window_size);
    void render(sf::RenderWindow &window);

    void playImpactSound();

    size_t getPelletCount() const { return pellets.size(); }
    size_t getPelletCapacity() const { return pellets.capacity(); }
};
