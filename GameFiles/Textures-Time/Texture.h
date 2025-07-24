#pragma once
#include <iostream>
#include <string>
#include <SFML/Graphics.hpp>

class Texture : public sf::Texture {
private:
    sf::Vector2u frame_size;
    sf::Vector2i texturepos;
    float frametime;
    float framemaxtime;

    void StartTexture(std::string address);
    void StartTime();

public:
    Texture(std::string address, sf::Vector2u size);
    ~Texture();

    void frameright();
    void frameleft();

    float getframetimer() const;
    void setframetimer(float frame_time);

    float getframetimerMax() const;
    void setframetimerMax(float frame_time);

    sf::Vector2i get_texpos() const;
    sf::Vector2u framesize() const;


    void animate(float dt);
    sf::Vector2f getTexturePosition() const;
};
