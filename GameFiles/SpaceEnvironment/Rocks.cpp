#include "SpaceEnvironment/Rocks.h"
#include "SpaceEnvironment/Pellets.h"
#include "Menu/Debug.h"
#include "SpaceEnvironment/Ship.h"
#include <algorithm>
#include <cmath>

void Rocks::initVariables() {
    maxRocks = 450;
    rockTimerMax = 0.6f;
    rockTimer = 0.0f;
    baseVelocity = 130.0f;
    textureSize = {96, 96};
    rockAnimationSpeed = 0.1f;
    detonationDuration = 0.7f;
}

void Rocks::initRandomize() {
    randomize = new Randomize();
}

void Rocks::initColors() {
    colors = new Colors();
}

sf::FloatRect Rocks::getRockBounds(Entity* rock) {
    sf::Vector2f pos = rock->getPosition();
    sf::Vector2f scale = rock->getScale();
    float actualRockSize = 40.0f;
    float scaledSize = actualRockSize * scale.x;

    return sf::FloatRect({pos.x - scaledSize/2.0f, pos.y - scaledSize/2.0f},
                         {scaledSize, scaledSize});
}

void Rocks::spawnRock(sf::Vector2f pos, sf::Vector2f window_size) {
    Entity* newRock = new Entity("GameResources/Sprites/AnimatedRock.png", textureSize);
    rocks.push_back(newRock);
    rockStates.push_back(RockState::STATIC);
    detonationTimers.push_back(0.0f);

    Entity* rock = rocks.back();

    rock->setFrameTimerMax(0.0f);
    rock->setHasLooping(false);

    sf::Vector2f startCoords = calculateFrameCoords(0);
    rock->shiftAnimation(startCoords);

    rock->setPosition(pos);
    rock->setOrigin(rock->getTextureSize() / 2.0f);
    rock->setColor(sf::Color::White);

    randomize->randomizeFloat(0.4f, 1.8f);
    float scale = randomize->randomF();

    randomize->randomizeFloat(0.0f, 1.0f);
    float sizeBias = randomize->randomF();

    if (sizeBias < 0.3f) {
        randomize->randomizeFloat(0.3f, 0.7f);
        scale = randomize->randomF();
    } else if (sizeBias < 0.6f) {
        randomize->randomizeFloat(0.8f, 1.2f);
        scale = randomize->randomF();
    } else {
        randomize->randomizeFloat(1.3f, 2.0f);
        scale = randomize->randomF();
    }

    rock->setScale(sf::Vector2f(scale, scale));

    randomize->randomizeFloat(-1.0f, 1.0f);
    float x = randomize->randomF();
    randomize->randomizeFloat(-1.0f, 1.0f);
    float y = randomize->randomF();

    if (abs(x) < 0.3f) x = (x >= 0) ? 0.5f : -0.5f;
    if (abs(y) < 0.3f) y = (y >= 0) ? 0.5f : -0.5f;

    sf::Vector2f vel = {x * baseVelocity, y * baseVelocity};
    rock->setVelocity(vel);

    rock->setOwnerTransformable(rock);
    rock->setHasLooping(false);
    rock->setHasGravity(false);
    rock->setFriction(0.0f);

    randomize->randomizeFloat(-2.0f, 2.0f);
    rock->setAngVelocity(randomize->randomF());
}

void Rocks::triggerRockDetonation(unsigned int index) {
    if (index >= rocks.size() || index >= rockStates.size()) return;

    rockStates[index] = RockState::DETONATING;

    if (index < detonationTimers.size()) {
        detonationTimers[index] = 0.0f;
    }

    rocks[index]->setFrameTimerMax(rockAnimationSpeed);
    rocks[index]->setHasLooping(false);

    sf::Vector2f startCoords = calculateFrameCoords(0);
    rocks[index]->shiftAnimation(startCoords);
}

void Rocks::updateRockAnimation(unsigned int index, float dt) {
    if (index >= rocks.size() || index >= rockStates.size()) return;

    Entity* rock = rocks[index];
    RockState& state = rockStates[index];

    switch (state) {
        case RockState::STATIC:
            break;

        case RockState::DETONATING:
            rock->animate(dt);

            if (index < detonationTimers.size()) {
                detonationTimers[index] += dt;

                if (detonationTimers[index] >= detonationDuration) {
                    state = RockState::DESTROYED;
                }
            }
            break;

        case RockState::DESTROYED:
            break;
    }
}

sf::Vector2f Rocks::calculateFrameCoords(int frameIndex) {
    int framesPerRow = 4;

    int row = frameIndex / framesPerRow;
    int col = frameIndex % framesPerRow;

    return sf::Vector2f(col * textureSize.x, row * textureSize.y);
}

int Rocks::getMaxFramesForTexture() {
    return 8;
}

void Rocks::deleteRock(unsigned int index) {
    if (index < rocks.size()) {
        delete rocks.at(index);
        rocks.erase(rocks.begin() + index);

        if (index < rockStates.size()) {
            rockStates.erase(rockStates.begin() + index);
        }
        if (index < detonationTimers.size()) {
            detonationTimers.erase(detonationTimers.begin() + index);
        }
    }
}

void Rocks::clearAllRocks() {
    for (int i = rocks.size() - 1; i >= 0; --i) {
        deleteRock(i);
    }
    rockStates.clear();
    detonationTimers.clear();
}

Rocks::Rocks() {
    initVariables();
    initRandomize();
    initColors();
}

Rocks::~Rocks() {
    clearAllRocks();
    delete randomize;
    delete colors;
}

void Rocks::update(float dt, sf::Vector2f window_size, Ship& ship, Pellets& pellets, std::uint16_t& score, int& health) {
    rockTimer -= dt;

    if (rockTimer <= 0.0f && rocks.size() < maxRocks) {
        randomize->randomizeInt(0, 3);
        int edge = randomize->randomI();

        sf::Vector2f pos;
        switch(edge) {
            case 0:
                randomize->randomizeFloat(0.0f, window_size.x);
                pos = {randomize->randomF(), -50.0f};
                break;
            case 1:
                randomize->randomizeFloat(0.0f, window_size.y);
                pos = {window_size.x + 50.0f, randomize->randomF()};
                break;
            case 2:
                randomize->randomizeFloat(0.0f, window_size.x);
                pos = {randomize->randomF(), window_size.y + 50.0f};
                break;
            case 3:
                randomize->randomizeFloat(0.0f, window_size.y);
                pos = {-50.0f, randomize->randomF()};
                break;
        }

        spawnRock(pos, window_size);
        rockTimer = rockTimerMax;
    }

    const sf::FloatRect shipBounds = ship.getGlobalBounds();
    const sf::FloatRect shieldBounds = ship.getShieldActive() ? ship.getShieldBounds() : sf::FloatRect();
    const bool hasActiveShield = ship.getShieldActive();

    std::vector<Entity*>& pelletList = pellets.getPellets();

    std::vector<int> rocksToRemove;
    std::vector<int> pelletsToRemove;
    rocksToRemove.reserve(rocks.size());
    pelletsToRemove.reserve(pelletList.size());

    std::vector<sf::FloatRect> pelletBounds;
    pelletBounds.reserve(pelletList.size());
    for (Entity* pellet : pelletList) {
        pelletBounds.push_back(pellets.getPelletBounds(pellet));
    }

    for (int rockIndex = rocks.size() - 1; rockIndex >= 0; --rockIndex) {
        Entity* rock = rocks[rockIndex];
        RockState& rockState = rockStates[rockIndex];

        rock->updateEntity(dt, window_size);
        updateRockAnimation(rockIndex, dt);

        if (rockState == RockState::DESTROYED) {
            rocksToRemove.push_back(rockIndex);
            continue;
        }

        const sf::FloatRect rockBounds = getRockBounds(rock);

        if (hasActiveShield && shieldBounds.findIntersection(rockBounds)) {
            ship.triggerShieldHit();
            ship.deactivateShield();

            if (rockState == RockState::STATIC) {
                triggerRockDetonation(rockIndex);
            }
        }
        else if (rockState == RockState::STATIC && shipBounds.findIntersection(rockBounds)) {
            float rockSize = rock->getScale().x;
            int damage = std::min(100, 25 + static_cast<int>(rockSize * 25));
            health -= damage;
            ship.playCollisionSound();

            triggerRockDetonation(rockIndex);
        }

        if (rockState == RockState::STATIC) {
            for (int pelletIndex = 0; pelletIndex < static_cast<int>(pelletList.size()); ++pelletIndex) {
                if (std::find(pelletsToRemove.begin(), pelletsToRemove.end(), pelletIndex) != pelletsToRemove.end()) {
                    continue;
                }

                if (rockBounds.findIntersection(pelletBounds[pelletIndex])) {
                    pellets.playImpactSound();
                    pelletsToRemove.push_back(pelletIndex);

                    triggerRockDetonation(rockIndex);

                    const float currentSize = rock->getScale().x * 40.0f;
                    if (currentSize >= 60.0f) {
                        const sf::Vector2f rockPos = rock->getPosition();
                        const sf::Color rockColor = rock->getColor();
                        const sf::Vector2f rockScale = rock->getScale();

                        rock->scale({0.8f, 0.8f});

                        if (rocks.size() < maxRocks) {
                            spawnRock(rockPos, window_size);
                            if (!rocks.empty()) {
                                const size_t newIndex = rocks.size() - 1;
                                rocks[newIndex]->setColor(rockColor);
                                rocks[newIndex]->setScale(rockScale * 0.8f);
                            }
                        }
                    }

                    score += 10;
                    break;
                }
            }
        }
    }

    std::sort(pelletsToRemove.rbegin(), pelletsToRemove.rend());
    std::sort(rocksToRemove.rbegin(), rocksToRemove.rend());

    for (int pelletIndex : pelletsToRemove) {
        if (pelletIndex < static_cast<int>(pelletList.size())) {
            delete pelletList[pelletIndex];
            pelletList.erase(pelletList.begin() + pelletIndex);
        }
    }

    for (int rockIndex : rocksToRemove) {
        if (rockIndex < static_cast<int>(rocks.size())) {
            deleteRock(rockIndex);
        }
    }
}

void Rocks::reserveCapacity(size_t capacity) {
    rocks.reserve(capacity);
    rockStates.reserve(capacity);
    detonationTimers.reserve(capacity);
}

void Rocks::render(sf::RenderWindow& window) {
    for (unsigned int i = 0; i < rocks.size(); ++i) {
        if (rocks.at(i)) {
            window.draw(*rocks.at(i));
        }
    }
}
