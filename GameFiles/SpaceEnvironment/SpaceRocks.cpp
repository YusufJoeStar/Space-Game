#include "SpaceEnvironment/SpaceRocks.h"
#include <math.h>
#include "Menu/Debug.h"

SpaceRocks::SpaceRocks(sf::Vector2f window_size) {
    dt = 0.0f;
    score = 0;
    StartScoreText(window_size);
    initHealth();
	initHealthbar(window_size);
    StartShip(window_size);
}

void SpaceRocks::initEndText(sf::Vector2f window_size) {

	endText = new Texts("Game Over: ", "GameResources/Fonts/ariblk.ttf");
	endText->setCharacterSize(window_size.x / 32.0f);
	endText->setOrigin(scoreText->getLocalBounds().size / 2.0f);
	endText->setPosition(window_size / 2.0f);
}

void SpaceRocks::StartScoreText(sf::Vector2f window_size) {
    scoreText = new Texts("Score:", "GameResources/Fonts/ariblk.ttf");
    scoreText->setCharacterSize(window_size.x / 32.0f);
    scoreText->setOrigin(scoreText->getLocalBounds().size / 2.0f);
    scoreText->setPosition({window_size.x / 10.0f, window_size.y / 25.0f});
}

void SpaceRocks::StartShip(sf::Vector2f window_size) {
    ship.setPosition(window_size / 2.0f);
}

void SpaceRocks::initHealth() {

	health = 1000;
}
void SpaceRocks::initHealthbar(sf::Vector2f window_size) {

	healthbar = new HealthBar(health, window_size);
}


void SpaceRocks::update(sf::Vector2f window_size) {
    times.deltareset();
    dt = times.getdelta();

    //DEBUG_LOG("[SpaceRocks] Update called with dt: " << dt);

    ship.update(dt, window_size);


    float shipRotation = ship.getRotation().asRadians();
    float firingAngle = shipRotation - (3.14159f / 2.0f);  // Subtract 90 degrees (π/2) to point forward


    sf::Vector2f shipPos = ship.getPosition();
    sf::Vector2f shipSize = ship.getTextureSize();
    sf::Vector2f forwardDirection = {cos(firingAngle), sin(firingAngle)};
    sf::Vector2f firingPosition = shipPos + (forwardDirection * (shipSize.x * 0.6f));

    //DEBUG_LOG("[SpaceRocks] Ship rotation: " << shipRotation << " Firing angle: " << firingAngle);
    //DEBUG_LOG("[SpaceRocks] Ship pos: (" << shipPos.x << ", " << shipPos.y << ") Firing pos: (" << firingPosition.x << ", " << firingPosition.y << ")");

    pellets.firePellet(firingAngle, firingPosition);
    pellets.update(dt, window_size);


    rocks.update(dt, window_size, ship, pellets, score, health);
    healthbar->updateHealth(health);

    //DEBUG_LOG("[SpaceRocks] Current rock count: " << rocks.getRockCount());
    //DEBUG_LOG("[SpaceRocks] Current score: " << score << " Health: " << health);

    scoreText->streamtext("Score: ", score);
}

void SpaceRocks::render(sf::RenderWindow &window) {
    window.draw(*scoreText);
    pellets.render(window);
    ship.render(window);
    rocks.render(window);
    healthbar->render(window);
    if(health <= 0) {

		window.draw(*endText);
	}
}


SpaceRocks::~SpaceRocks() {
    delete scoreText;
    delete endText;
    delete healthbar;
}
