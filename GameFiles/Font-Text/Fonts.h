#pragma once
#include <iostream>
#include <string>
#include <SFML/Graphics.hpp>

class Fonts:public sf::Font{
private:
    void fontform(std::string address);

public:
    Fonts(std::string address);
    ~Fonts();
};
