#include "Textures-Time/Sprites.h"
#include "Menu/Debug.h"





Sprites::Sprites(std::string spriteaddress,sf::Vector2u spritesize):sf::Sprite(temptexture) {



textures= new Texture(spriteaddress,spritesize);
//DEBUG_LOG("[Sprite][INFO] Setting texture...");


this->setTexture(*textures);
this->setOrigin({spritesize.x/2.0f , spritesize.y/2.0f});
this->setTextureRect({{0,0},sf::Vector2i(spritesize)});


//DEBUG_LOG("[Sprite][INFO] Full texture size: " << textures->getSize().x << "x" << textures->getSize().y);
//DEBUG_LOG("[Sprite][INFO] Frame size: " << textures->framesize().x << "x" << textures->framesize().y);


}

Sprites::~Sprites() {

delete textures;

}





void Sprites::animate(float dt) {

textures->setframetimer(textures->getframetimer() - dt);



if(textures->getframetimer() <= 0.0f){

    textures->frameright();
    this->setTextureRect(sf::IntRect(textures->get_texpos(), sf::Vector2i(textures->framesize())));
    textures->setframetimer(textures->getframetimerMax());
}



//DEBUG_LOG("[Sprite][ANIM] animate() CALLED");

}
