#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include "Textures-Time/Time.h"
#include <iostream>
#include "Font-Text/Fonts.h"
#include "Menu/Images.h"
#include "Textures-Time/Sprites.h"
#include "SpaceEnvironment/SpaceRocks.h"
#include "Menu/Menu.h"

enum class GameState {
    MENU,
    PLAYING,
    PAUSED,
    GAME_OVER
};

class Game{
private:
    sf::Vector2f windowSize;
    std::string title;
    sf::VideoMode vm;
    sf::RenderWindow *window;
    Time time;
    Texts *text;
    Images *images;
    Sprites *sprites;
    SpaceRocks *spaceRocks;
    Rocks *rocks;
    Menu *menu;

    GameState currentGameState;

    void StartWindow();
    void StartSpaceRocks();
    void StartGui();
    void StartText();
    void StartImages();
    void StartSprites();
    void StartMenu();

    void PollEvents();
    void update();
    void render();

    void updateGame();
    void renderGame();

    void updateMenu();
    void renderMenu();

public:
    Game();
    ~Game();

    void run();
};
