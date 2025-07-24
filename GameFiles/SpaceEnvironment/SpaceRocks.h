#pragma once
#include "Font-Text/Texts.h"
#include "Textures-Time/Time.h"
#include "SpaceEnvironment/Ship.h"
#include "SpaceEnvironment/Pellets.h"
#include "SpaceEnvironment/Rocks.h"
#include "SpaceEnvironment/HealthBar.h"

class SpaceRocks{

private:

Time times;
Texts *scoreText;
Texts *endText;
Ship ship;
Pellets pellets;
Rocks rocks;
HealthBar *healthbar;

float dt;
std::uint16_t score;
int health;

void StartScoreText(sf::Vector2f window_size);
void initEndText(sf::Vector2f window_size);
void StartShip(sf::Vector2f window_size);
void initHealth();
void initHealthbar(sf::Vector2f window_size);

public:

SpaceRocks(sf::Vector2f window_size);
~SpaceRocks();

void update(sf::Vector2f window_size);
void render(sf::RenderWindow &window);

};
