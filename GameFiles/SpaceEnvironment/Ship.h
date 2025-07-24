#pragma once
#include "SpaceEnvironment/Entity.h"
#include "Audio-SoundEffects/SoundEffects.h"
#include <SFML/Graphics.hpp>

class Ship : public Entity {
private:
    const float halfPi = 1.57079632679f;

    float thrust;
    float turn;

    Entity* boostSprite;
    bool isBoosting;
    float boostMultiplier;

    Entity* shieldSprite;
    bool isShieldActive;
    bool wasShieldActiveLastFrame;
    bool fPressedLastFrame;
    float shieldRadius;
    sf::Color shieldDefaultColor;
    sf::Color shieldHitColor;
    float shieldHitTimer;
    float shieldHitDuration;
    bool shieldHitFlash;

    Sound* soundEffects;
    bool wasMovingLastFrame;
    bool wasBoostingLastFrame;
    bool wasRotatingLastFrame;
    bool wasIdleLastFrame;

    bool aPressed = false;
    bool wPressed = false;
    bool dPressed = false;
    bool sPressed = false;
    bool spacePressed = false;
    bool fPressed = false;

    void StartVariables();
    void StartSounds();
    void StartShield();
    void MoveShip(float dt);
    void UpdateBoost(float dt);
    void UpdateShield(float dt);
    void UpdateSounds(float dt);

public:
    Ship();
    ~Ship();

    void update(float dt, sf::Vector2f window_size);
    void render(sf::RenderWindow& window);

    bool getShieldActive() const { return isShieldActive; }
    float getShieldRadius() const { return shieldRadius; }
    void triggerShieldHit();
    void deactivateShield();
    void playCollisionSound();
    sf::FloatRect getShieldBounds() const;
};
