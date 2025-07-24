#include "Font-Text/Fonts.h"
#include "Menu/Debug.h"

void Fonts::fontform(std::string address) {
    if(!this->openFromFile(address)){
        DEBUG_LOG("Error, can't load file address");
    }
}

Fonts::Fonts(std::string address){
    fontform(address);
}

Fonts::~Fonts(){}
