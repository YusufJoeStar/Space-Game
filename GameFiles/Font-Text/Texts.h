#pragma once
#include <sstream>
#include "Font-Text/Fonts.h"

class Texts:public sf::Text{
private:
std::string originalstr;
Fonts *fonts;

void firstfont(std::string fontaddress);
void firststring(std::string str);

public:

Texts(std::string str,std::string fontaddress);
~Texts();

void streamtext(std::string str="",float var=0.0f);
};
