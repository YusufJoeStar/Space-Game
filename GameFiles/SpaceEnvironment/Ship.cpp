#include "SpaceEnvironment/Ship.h"
#include "Physics-Colors/Physics.h"
#include "Menu/Debug.h"
#include <iostream>
#include <math.h>

Ship::Ship() : Entity("GameResources/Sprites/SpareShip.png", {32, 40}) {
    StartVariables();
    StartShield();
    StartSounds();
}

Ship::~Ship() {
    delete boostSprite;
    delete shieldSprite;
    delete soundEffects;
}

void Ship::StartVariables() {
    this->setOwnerTransformable(this);
    this->setFriction(0.05f);
    this->setAngFriction(0.125f);
    this->setHasLooping(true);
    this->setFrameTimerMax(0.3f);
    this->setOrigin(this->size / 2.0f);
    this->setScale(sf::Vector2f(2.0f, 2.0f));

    this->shiftAnimation({0.0f, 0.0f});

    thrust = 50.0f;
    turn = 900.0f;

    boostSprite = new Entity("Sprites/thrust.png", {0, 0});
    boostSprite->setOrigin(boostSprite->getTextureSize() / 2.0f);
    boostSprite->setFrameTimerMax(0.0f);
    boostSprite->scale({0.1f, 0.1f});
    isBoosting = false;
    boostMultiplier = 2.5f;

    wasMovingLastFrame = false;
    wasBoostingLastFrame = false;
    wasRotatingLastFrame = false;
    wasShieldActiveLastFrame = false;
    fPressedLastFrame = false;
}

void Ship::playCollisionSound() {
    soundEffects->playOneShotEffect(SoundEffect::SHIP_ROCK_COLLISION, 80.0f);
}

void Ship::StartShield() {
    DEBUG_LOG("[Ship][SHIELD] Initializing shield system");

    shieldSprite = new Entity("GameResources/Sprites/ShipShield.png", {64, 64});
    shieldSprite->setOrigin(shieldSprite->getTextureSize() / 2.0f);
    shieldSprite->setFrameTimerMax(0.1f);
    shieldSprite->scale({1.5f, 1.5f});

    isShieldActive = false;
    wasShieldActiveLastFrame = false;
    fPressedLastFrame = false;
    shieldRadius = 48.0f;

    shieldDefaultColor = sf::Color(0, 150, 255, 180);
    shieldHitColor = sf::Color(255, 100, 100, 220);
    shieldHitTimer = 0.0f;
    shieldHitDuration = 0.15f;
    shieldHitFlash = false;

    shieldSprite->setColor(shieldDefaultColor);
}

void Ship::StartSounds() {
    soundEffects = new Sound();

    soundEffects->loadSoundEffect(SoundEffect::SHIP_THRUST_NORMAL, "GameResources/Assets/TemporaryEngine.ogg");
    soundEffects->loadSoundEffect(SoundEffect::SHIP_THRUST_BOOST, "GameResources/Assets/EngineBoost.mp3");
    soundEffects->loadSoundEffect(SoundEffect::SHIP_ROTATION, "GameResources/Assets/EngineRotate.mp3");
    soundEffects->loadSoundEffect(SoundEffect::ENGINE_IDLE, "GameResources/Assets/EngineStartup.mp3");
    soundEffects->loadSoundEffect(SoundEffect::SHIP_ROCK_COLLISION, "GameResources/Assets/ShipHit.ogg");

    soundEffects->loadSoundEffect(SoundEffect::SHIELD_ACTIVATE, "GameResources/Assets/ShieldActivate.ogg");
    soundEffects->loadSoundEffect(SoundEffect::SHIELD_DEACTIVATE, "GameResources/Assets/ShieldDeactivate.ogg");
    soundEffects->loadSoundEffect(SoundEffect::SHIELD_HUM, "GameResources/Assets/ShieldHum.ogg");
    soundEffects->loadSoundEffect(SoundEffect::SHIELD_HIT, "GameResources/Assets/ShieldHit.ogg");

    soundEffects->setMaxVolume(SoundEffect::SHIP_THRUST_NORMAL, 20.0f);
    soundEffects->setMaxVolume(SoundEffect::SHIP_THRUST_BOOST, 55.0f);
    soundEffects->setMaxVolume(SoundEffect::SHIP_ROTATION, 10.0f);
    soundEffects->setMaxVolume(SoundEffect::ENGINE_IDLE, 30.0f);
    soundEffects->setMaxVolume(SoundEffect::SHIP_ROCK_COLLISION, 50.0f);

    soundEffects->setMaxVolume(SoundEffect::SHIELD_ACTIVATE, 40.0f);
    soundEffects->setMaxVolume(SoundEffect::SHIELD_DEACTIVATE, 40.0f);
    soundEffects->setMaxVolume(SoundEffect::SHIELD_HUM, 40.0f);
    soundEffects->setMaxVolume(SoundEffect::SHIELD_HIT, 40.0f);

    soundEffects->setFadeSpeed(SoundEffect::SHIP_THRUST_NORMAL, 120.0f);
    soundEffects->setFadeSpeed(SoundEffect::SHIP_THRUST_BOOST, 100.0f);
    soundEffects->setFadeSpeed(SoundEffect::SHIP_ROTATION, 120.0f);
    soundEffects->setFadeSpeed(SoundEffect::ENGINE_IDLE, 80.0f);
    soundEffects->setFadeSpeed(SoundEffect::SHIELD_HIT, 70.0f);

    soundEffects->setFadeSpeed(SoundEffect::SHIELD_HUM, 90.0f);

    soundEffects->startSoundEffect(SoundEffect::ENGINE_IDLE, true);
    soundEffects->setTargetVolume(SoundEffect::ENGINE_IDLE, 15.0f);
}

void Ship::MoveShip(float dt) {
    aPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::A);
    wPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::W);
    sPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::S);
    dPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::D);
    bool shiftPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::LShift);
    fPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::F);

    isBoosting = shiftPressed && (wPressed || sPressed);

    if (fPressed && !fPressedLastFrame) {
        isShieldActive = !isShieldActive;
    }
    fPressedLastFrame = fPressed;

    if (aPressed || wPressed || sPressed || dPressed) {
        float torque = turn * (dPressed - aPressed);
        this->addTorque(torque);

        float r = this->getRotation().asRadians() - halfPi;
        sf::Vector2f dir = {std::cos(r), std::sin(r)};

        float thrustInput = (float(wPressed) - float(sPressed));

        float currentThrust = thrust;
        if (isBoosting && thrustInput != 0.0f) {
            currentThrust *= boostMultiplier;
        }

        dir *= currentThrust * thrustInput;
        this->addForce(dir);

        this->animate(dt);
    } else {
        static bool wasMovingPreviously = false;
        if (wasMovingPreviously) {
            this->shiftAnimation({0.0f, 0.0f});
        }
        wasMovingPreviously = false;
        return;
    }
}

void Ship::UpdateBoost(float dt) {
    if (isBoosting) {
        sf::Transform shipTransform = this->getTransform();

        sf::Vector2f localBoostOffset(0.0f, this->size.y * 0.3f);

        sf::Vector2f worldBoostPosition = shipTransform.transformPoint(localBoostOffset);

        boostSprite->setPosition(worldBoostPosition);
        boostSprite->setRotation(this->getRotation());

        sf::Vector2f shipCenter = this->getPosition();
        float shipRotation = this->getRotation().asRadians();

        sf::Vector2f backwardOffset;
        backwardOffset.x = -sin(shipRotation) * (this->size.y * 0.6f);
        backwardOffset.y = cos(shipRotation) * (this->size.y * 0.6f);

        sf::Vector2f boostPosition = shipCenter + backwardOffset;
        boostSprite->setPosition(boostPosition);
        boostSprite->setRotation(this->getRotation());

        boostSprite->animate(dt);
    }
}

void Ship::UpdateShield(float dt) {
    if (isShieldActive) {
        shieldSprite->setPosition(this->getPosition());
        shieldSprite->setRotation(this->getRotation());

        shieldSprite->animate(dt);
    }

    if (shieldHitFlash) {
        shieldHitTimer -= dt;

        if (shieldHitTimer <= 0.0f) {
            shieldHitFlash = false;
            shieldSprite->setColor(shieldDefaultColor);
            DEBUG_LOG("[Ship][SHIELD] Hit flash effect ended");
        } else {
            float flashPhase = sin(shieldHitTimer * 20.0f);
            if (flashPhase > 0.0f) {
                shieldSprite->setColor(shieldHitColor);
            } else {
                shieldSprite->setColor(shieldDefaultColor);
            }
        }
    }
}

void Ship::UpdateSounds(float dt) {
    bool isMoving = wPressed || sPressed;
    bool isRotating = aPressed || dPressed;
    bool isCurrentlyBoosting = isBoosting;

    if (isMoving) {
        soundEffects->setTargetVolume(SoundEffect::SHIP_THRUST_NORMAL, 50.0f);
        soundEffects->setTargetVolume(SoundEffect::ENGINE_IDLE, 5.0f);
    } else {
        soundEffects->setTargetVolume(SoundEffect::SHIP_THRUST_NORMAL, 0.0f);
        soundEffects->setTargetVolume(SoundEffect::ENGINE_IDLE, 25.0f);
    }

    if (isCurrentlyBoosting) {
        soundEffects->setTargetVolume(SoundEffect::SHIP_THRUST_BOOST, 50.0f);
    } else {
        soundEffects->setTargetVolume(SoundEffect::SHIP_THRUST_BOOST, 0.0f);
    }

    if (isMoving && isRotating) {
        soundEffects->setTargetVolume(SoundEffect::SHIP_ROTATION, 10.0f);
    } else {
        soundEffects->setTargetVolume(SoundEffect::SHIP_ROTATION, 0.0f);
    }

    if (isShieldActive && !wasShieldActiveLastFrame) {
        soundEffects->playOneShotEffect(SoundEffect::SHIELD_ACTIVATE, 40.0f);
        soundEffects->startSoundEffect(SoundEffect::SHIELD_HUM, true);
        soundEffects->setTargetVolume(SoundEffect::SHIELD_HUM, 40.0f);
    }
    else if (!isShieldActive && wasShieldActiveLastFrame) {
        soundEffects->playOneShotEffect(SoundEffect::SHIELD_DEACTIVATE, 40.0f);
        soundEffects->setTargetVolume(SoundEffect::SHIELD_HUM, 0.0f);
    }
    else if (isShieldActive) {
        soundEffects->setTargetVolume(SoundEffect::SHIELD_HUM, 40.0f);
    }
    else {
        soundEffects->setTargetVolume(SoundEffect::SHIELD_HUM, 0.0f);
    }

    soundEffects->update(dt);

    wasMovingLastFrame = isMoving;
    wasBoostingLastFrame = isCurrentlyBoosting;
    wasRotatingLastFrame = isRotating;
    wasShieldActiveLastFrame = isShieldActive;
}

void Ship::triggerShieldHit() {
    if (isShieldActive) {
        shieldHitFlash = true;
        shieldHitTimer = shieldHitDuration;
        shieldSprite->setColor(shieldHitColor);

        soundEffects->playOneShotEffect(SoundEffect::SHIELD_HIT, 20.0f);
    }
}

void Ship::deactivateShield() {
    if (isShieldActive) {
        triggerShieldHit();

        isShieldActive = false;

        soundEffects->setTargetVolume(SoundEffect::SHIELD_HUM, 0.0f);
        soundEffects->playOneShotEffect(SoundEffect::SHIELD_DEACTIVATE, 50.0f);
    }
}

sf::FloatRect Ship::getShieldBounds() const {
    if (!isShieldActive) {
        return sf::FloatRect();
    }

    sf::Vector2f pos = this->getPosition();
    float radius = shieldRadius;

    return sf::FloatRect(
        sf::Vector2f(pos.x - radius, pos.y - radius),
        sf::Vector2f(radius * 2.0f, radius * 2.0f)
    );
}

void Ship::update(float dt, sf::Vector2f window_size) {
    MoveShip(dt);
    UpdateBoost(dt);
    UpdateShield(dt);
    UpdateSounds(dt);
    this->updateEntity(dt, window_size);
}

void Ship::render(sf::RenderWindow& window) {
    if (isBoosting) {
        window.draw(*boostSprite);
    }

    window.draw(*this);

    if (isShieldActive) {
        window.draw(*shieldSprite);
    }
}
