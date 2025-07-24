#include "Textures-Time/Texture.h"
#include "Menu/Debug.h"

void Texture::StartTexture(std::string address) {
    if (!this->loadFromFile(address)) {
        //DEBUG_LOG("[Texture][ERROR] Could not load file");
    } else {
        //DEBUG_LOG("[Texture][INFO] Texture loaded successfully from " + address);
    }
}

Texture::Texture(std::string address, sf::Vector2u size) {
    StartTime();
    texturepos = {0, 0};
    frame_size = size;
    StartTexture(address);
}

void Texture::StartTime() {
    frametime = 0.0f;
    framemaxtime = 0.0f;
}

float Texture::getframetimer() const {
    return frametime;
}

float Texture::getframetimerMax() const {
    return framemaxtime;
}

void Texture::setframetimerMax(float frame_time) {
    framemaxtime = frame_time;
}

void Texture::setframetimer(float frame_time) {
    frametime = frame_time;
}

sf::Vector2i Texture::get_texpos() const {
    return texturepos;
}

sf::Vector2u Texture::framesize() const {
    return frame_size;
}

void Texture::frameleft() {
    texturepos.x -= frame_size.x;
    if (texturepos.x < 0) {
        texturepos.x = getSize().x - frame_size.x;
    }
}

void Texture::frameright() {
    //DEBUG_LOG("[Texture][ANIM] Current frame pos: (" << texturepos.x << ", " << texturepos.y << ")");
    //DEBUG_LOG("[Texture][ANIM] Frame size: " << frame_size.x << "x" << frame_size.y);
   //DEBUG_LOG("[Texture][ANIM] Texture size: " << getSize().x << "x" << getSize().y);


    texturepos.x += frame_size.x;


    if (texturepos.x >= static_cast<int>(getSize().x)) {
        texturepos.x = 0;
        texturepos.y += frame_size.y;


        if (texturepos.y >= static_cast<int>(getSize().y)) {
            texturepos.y = 0;
        }
    }

    //DEBUG_LOG("[Texture][ANIM] New frame pos: (" << texturepos.x << ", " << texturepos.y << ")");
}

void Texture::animate(float dt) {
    frametime += dt;
    if (frametime >= framemaxtime) {
        frameright();
        frametime = 0.0f;
    }
}

sf::Vector2f Texture::getTexturePosition() const {
    return static_cast<sf::Vector2f>(texturepos);
}


Texture::~Texture() {}
