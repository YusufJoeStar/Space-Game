#include "SpaceEnvironment/Pellets.h"
#include <math.h>
#include "Menu/Debug.h"

Pellets::Pellets() {
    pelletTimerMax = 0.2f;
    pelletTimer = pelletTimerMax;
    baseVelocity = 700.0f;
    maxPellets = 9;
    StartSounds();
}

Pellets::~Pellets() {
    for (int i = pellets.size()-1; i >= 0; --i) {
        if (pellets[i]) {
            delete pellets[i];
            pellets[i] = nullptr;
        }
    }
    pellets.clear();

    if (soundEffects) {
        delete soundEffects;
        soundEffects = nullptr;
    }
}

void Pellets::StartSounds() {
    soundEffects = new Sound();

    soundEffects->loadSoundEffect(SoundEffect::PELLET_FIRE, "GameResources/Assets/RockHit.ogg");
    soundEffects->loadSoundEffect(SoundEffect::PELLET_IMPACT, "GameResources/Assets/RockStruck.ogg");

    soundEffects->setMaxVolume(SoundEffect::PELLET_FIRE, 20.0f);
    soundEffects->setMaxVolume(SoundEffect::PELLET_IMPACT, 20.0f);
}

void Pellets::playImpactSound() {
    soundEffects->playOneShotEffect(SoundEffect::PELLET_IMPACT, 10.0f);
}

void Pellets::OutOfBounds(unsigned int index, sf::Vector2f window_size) {
    if (index >= pellets.size()) return;

    sf::Vector2f pos = pellets.at(index)->getPosition();
    sf::Vector2f texSize = pellets.at(index)->getTextureSize();

    if ((pos.x < -texSize.x) ||
        (pos.x > window_size.x + texSize.x) ||
        (pos.y < -texSize.y) ||
        (pos.y > window_size.y + texSize.y)) {

        delete pellets.at(index);
        pellets.erase(pellets.begin() + index);
    }
}

sf::FloatRect Pellets::getPelletBounds(Entity* pellet) {
    sf::Vector2f pos = pellet->getPosition();
    float size = 12.0f;

    return sf::FloatRect({pos.x - size/2.0f, pos.y - size/2.0f}, {size, size});
}

void Pellets::firePellet(float angle, sf::Vector2f position) {
    if ((pelletTimer <= 0.0f) && (pellets.size() < maxPellets) &&
        (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Space))) {

        soundEffects->playOneShotEffect(SoundEffect::PELLET_FIRE, 20.0f);

        pellets.push_back(new Entity("GameResources/Sprites/Pellets1.png", {13, 13}));
        unsigned int index = pellets.size() - 1;
        Entity* pellet = pellets.at(index);

        setPelletSprite(pellet, 4, 2);

        sf::Vector2f direction = {cos(angle), sin(angle)};

        pellet->setOrigin(pellet->getTextureSize() / 2.0f);
        pellet->setPosition(position);

        pellet->setOwnerTransformable(pellet);
        pellet->setVelocity(baseVelocity * direction);
        pellet->setFriction(0.0f);
        pellet->setHasGravity(false);
        pellet->setHasLooping(false);

        pellet->setFrameTimerMax(0.8f);

        pelletTimer = pelletTimerMax;
    }
}

void Pellets::setPelletSprite(Entity* pellet, int column, int row) {
    sf::Vector2f spritePos(column * 13.0f, row * 12.0f);

    pellet->shiftAnimation(spritePos);
}

void Pellets::update(float dt, sf::Vector2f window_size) {
    pelletTimer -= dt;

    if (soundEffects) {
        soundEffects->update(dt);
    }

    std::vector<int> indicesToRemove;

    for (int i = 0; i < static_cast<int>(pellets.size()); ++i) {
        if (pellets[i]) {
            pellets[i]->updateEntity(dt, window_size);

            sf::Vector2f pos = pellets[i]->getPosition();
            sf::Vector2f texSize = pellets[i]->getTextureSize();

            if ((pos.x < -texSize.x) ||
                (pos.x > window_size.x + texSize.x) ||
                (pos.y < -texSize.y) ||
                (pos.y > window_size.y + texSize.y)) {

                indicesToRemove.push_back(i);
            }
        }
    }

    for (int i = indicesToRemove.size() - 1; i >= 0; --i) {
        int index = indicesToRemove[i];
        if (index < static_cast<int>(pellets.size()) && pellets[index]) {
            delete pellets[index];
            pellets.erase(pellets.begin() + index);
        }
    }
}

void Pellets::render(sf::RenderWindow &window) {
    for (unsigned int i = 0; i < pellets.size(); ++i) {
        window.draw(*pellets.at(i));
    }
}

std::vector<Entity*> &Pellets::getPellets() {
    return pellets;
}
