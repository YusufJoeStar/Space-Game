#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

class Images : public sf::Image {
private:
    sf::Texture* texture;
    sf::Sprite* sprite;
    std::vector<std::string> imageFiles;
    std::string currentImageFile;

    void StartTexture();

public:
    Images(const std::string& filepath);
    Images();

    void addImageFile(const std::string& filepath);
    bool loadRandomImage();
    void clearImageList();

    ~Images();

    void UpdateTexture();
    sf::Sprite& getSprite();
    void StartSprite(sf::Vector2u windowSize);
};
