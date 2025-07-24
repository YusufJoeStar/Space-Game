#pragma once
#include "Textures-Time/Texture.h"

class Sprites:public sf::Sprite{

private:


sf::Texture temptexture;




public:

Texture *textures;

Sprites(std::string spriteaddress="",sf::Vector2u spritesize={16,16});


~Sprites();





void animate(float dt=0.16667);
};
