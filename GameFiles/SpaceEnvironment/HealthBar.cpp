#include "SpaceEnvironment/HealthBar.h"
#include "Menu/Debug.h"

void HealthBar::initBackground() {
    background.setFillColor({ 120, 20, 20, 180 });
    background.setSize(healthSize);
    background.setPosition(this->getPosition());
}

void HealthBar::initBorder() {
    border.setFillColor(sf::Color::Transparent);
    border.setOutlineColor({ 220, 220, 220, 255 });
    border.setOutlineThickness(2.0f);
    border.setSize(healthSize);
    border.setPosition(this->getPosition());
}

HealthBar::HealthBar(int health, sf::Vector2f window_size) {
    maxHealth = health;
    currentHealth = health;

    healthSize.x = 200.0f;
    healthSize.y = 20.0f;

    sf::Vector2f position = {
        window_size.x - healthSize.x - 20.0f,
        20.0f
    };

    this->setFillColor(sf::Color::Green);
    this->setSize(healthSize);
    this->setPosition(position);

    initBackground();
    initBorder();
}

HealthBar::~HealthBar() {
}

void HealthBar::updateHealth(int health) {
    currentHealth = health;

    if (currentHealth < 0) currentHealth = 0;
    if (currentHealth > maxHealth) currentHealth = maxHealth;

    float healthPercent = float(currentHealth) / float(maxHealth);

    float newWidth = healthSize.x * healthPercent;
    this->setSize({ newWidth, healthSize.y });

    if (healthPercent > 0.6f) {
        this->setFillColor(sf::Color::Green);
    } else if (healthPercent > 0.3f) {
        this->setFillColor(sf::Color::Yellow);
    } else {
        this->setFillColor(sf::Color::Red);
    }
}

void HealthBar::render(sf::RenderWindow &window) {
    window.draw(background);
    window.draw(*this);
    window.draw(border);
}
