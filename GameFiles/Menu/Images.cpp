#include "Menu/Images.h"
#include "Physics-Colors/Randomize.h"
#include "Menu/Debug.h"
#include <iostream>

void Images::StartTexture() {
    texture = new sf::Texture(this->getSize());
}

void Images::StartSprite(sf::Vector2u windowSize) {
    sprite = new sf::Sprite(*texture);

    sf::Vector2u imageSize = texture->getSize();
    float scaleX = static_cast<float>(windowSize.x) / imageSize.x;
    float scaleY = static_cast<float>(windowSize.y) / imageSize.y;
    float scale = std::max(scaleX, scaleY);

    sprite->setScale({scale, scale});

    sf::FloatRect bounds = sprite->getGlobalBounds();
    sprite->setPosition({
        (windowSize.x - bounds.size.x) / 2.0f,
        (windowSize.y - bounds.size.y) / 2.0f
    });
}

Images::Images(const std::string& filepath) {
    texture = new sf::Texture();
    if (!texture->loadFromFile(filepath)) {

    } else {
        DEBUG_LOG("[Images] Image loaded successfully: " << filepath);
        currentImageFile = filepath;
    }

    sprite = new sf::Sprite(*texture);
}

Images::Images() {
    texture = new sf::Texture();
    sprite = nullptr;
}

Images::~Images() {
    delete texture;
    if (sprite != nullptr) {
        delete sprite;
    }
}

void Images::addImageFile(const std::string& filepath) {
    imageFiles.push_back(filepath);
}

bool Images::loadRandomImage() {
    if (imageFiles.empty()) {
        return false;
    }

    Randomize randomizer;
    randomizer.randomizeInt(0, imageFiles.size() - 1);
    int randomIndex = randomizer.randomI();

    std::string selectedFile = imageFiles[randomIndex];

    if (!texture->loadFromFile(selectedFile)) {
        return false;
    }

    currentImageFile = selectedFile;

    if (sprite == nullptr) {
        sprite = new sf::Sprite(*texture);
    } else {
        sprite->setTexture(*texture);
    }

    return true;
}

void Images::clearImageList() {
    imageFiles.clear();
}

void Images::UpdateTexture() {
    texture->update(*this);
}

sf::Sprite& Images::getSprite() {
    return *sprite;
}
