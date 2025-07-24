#include "Font-Text/Texts.h"
#include "Menu/Debug.h"
#include <SFML/Graphics.hpp>


void Texts::firstfont(std::string fontaddress){

fonts=new Fonts(fontaddress);
this->setFont(*fonts);

}

void Texts::firststring(std::string str) {

originalstr=str;
this->setString(originalstr);

}


Texts::Texts(std::string str,std::string fontaddress):sf::Text(*fonts) {

firstfont(fontaddress);
firststring(str);

}

Texts::~Texts() {
delete fonts;
}

void Texts::streamtext(std::string str,float var) {

std::stringstream streaming;
streaming<<str<<var;

this->setString(streaming.str());


}

