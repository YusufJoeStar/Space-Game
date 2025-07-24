#include "Menu/game.h"
#include <optional>
#include "Menu/Debug.h"
#include <SFML/Window/Event.hpp>

Game::Game() {
    DEBUG_LOG("[Game] Constructor start");
    currentGameState = GameState::MENU;

    StartWindow();
    StartMenu();

    text = nullptr;
    images = nullptr;
    sprites = nullptr;
    spaceRocks = nullptr;
    rocks = nullptr;
}

Game::~Game() {
    DEBUG_LOG("[Game] Destructor start");
    delete menu;

    if (images) delete images;
    if (text) delete text;
    if (sprites) delete sprites;
    if (spaceRocks) delete spaceRocks;
    if (rocks) delete rocks;

    delete window;
}

void Game::StartWindow() {
    DEBUG_LOG("[Game] Initializing window");
    windowSize.x = 1600.0f;
    windowSize.y = 900.0f;
    title = "Space Game";
    vm = sf::VideoMode(sf::Vector2u(windowSize));
    window = new sf::RenderWindow(vm, title);
    window->setFramerateLimit(60);
}

void Game::StartMenu() {
    DEBUG_LOG("[Game] Initializing menu");
    menu = new Menu(window, windowSize);
}

void Game::StartSpaceRocks() {
    if (!spaceRocks) {
        spaceRocks = new SpaceRocks(windowSize);
    }
}

void Game::StartText() {
    if (!text) {
        text = new Texts("dt=0.0000", "GameResources/Fonts/ariblk.ttf");
    }
}

void Game::StartImages() {
    if (!images) {
        DEBUG_LOG("[Game] Initializing images");

        images = new Images();

        images->addImageFile("GameResources/Resources/blue-preview.png");
        images->addImageFile("GameResources/Resources/BackgroundSpace3.png");
        images->addImageFile("GameResources/Resources/SpaceBackground2.png");
        images->addImageFile("GameResources/Resources/SpaceBackround.png");

        if (!images->loadRandomImage()) {
            DEBUG_LOG("[Game][ERROR] Random background loading failed!");
        } else {
            DEBUG_LOG("[Game] Random background loaded successfully!");
        }

        images->StartSprite(window->getSize());
    }
}

void Game::StartSprites() {
    if (!sprites) {
        sprites = new Sprites("Sprites/", {0, 0});
        sprites->setPosition({windowSize.x / 2.0f, windowSize.y / 2.0f});
        sprites->textures->setframetimer(0.15f);
    }
}

void Game::updateMenu() {
    time.deltareset();
    float dt = time.getdelta();

    MenuState menuResult = menu->handleEvents();
    MenuState currentMenuState = menu->getCurrentState();

    switch (currentMenuState) {
        case MenuState::TRANSITIONING_TO_GAME:
            DEBUG_LOG("[Game] In transition to game");
            menu->update(dt);
            break;

        case MenuState::GAME:
            DEBUG_LOG("[Game] Switching to game state");
            currentGameState = GameState::PLAYING;

            StartText();
            StartImages();
            StartSprites();
            StartSpaceRocks();
            break;

        case MenuState::TRANSITIONING_TO_EXIT:
            DEBUG_LOG("[Game] In transition to exit");
            menu->update(dt);
            break;

        case MenuState::EXIT:
            DEBUG_LOG("[Game] Exit requested from menu");
            window->close();
            break;

        case MenuState::MENU:
            menu->update(dt);
            break;
    }
}

void Game::renderMenu() {
    menu->render();
}

void Game::PollEvents() {
    DEBUG_LOG("[Game] Polling window events");
    while (const std::optional event = window->pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            DEBUG_LOG("[Game] Window closed event detected");
            window->close();
        }
        else if (const auto* keypressed = event->getIf<sf::Event::KeyPressed>()) {
            if (keypressed->scancode == sf::Keyboard::Scancode::Escape) {
                if (currentGameState == GameState::PLAYING) {
                    DEBUG_LOG("[Game] Escape pressed - returning to menu");
                    currentGameState = GameState::MENU;
                    menu->resetToMenu();
                } else {
                    DEBUG_LOG("[Game] Escape key pressed - closing");
                    window->close();
                }
            }
        }
    }
}

void Game::updateGame() {
    PollEvents();
    time.deltareset();
    float dt = time.getdelta();

    if (text) text->streamtext("dt: ", dt);
    if (sprites) sprites->animate(time.getdelta());
    if (spaceRocks) spaceRocks->update(windowSize);
}

void Game::renderGame() {
    DEBUG_LOG("[Game] --------------------------------------------------------------------- Rendering frame");
    window->clear();

    if (images) window->draw(images->getSprite());
    if (text) window->draw(*text);
    if (sprites) window->draw(*sprites);
    if (spaceRocks) spaceRocks->render(*window);

    window->display();
}

void Game::update() {
    switch (currentGameState) {
        case GameState::MENU:
            updateMenu();
            break;

        case GameState::PLAYING:
            updateGame();
            break;

        case GameState::PAUSED:
            break;

        case GameState::GAME_OVER:
            break;
    }
}

void Game::render() {
    switch (currentGameState) {
        case GameState::MENU:
            renderMenu();
            break;

        case GameState::PLAYING:
            renderGame();
            break;

        case GameState::PAUSED:
            break;

        case GameState::GAME_OVER:
            break;
    }
}

void Game::run() {
    DEBUG_LOG("[Game] Entering game loop");
    while (window->isOpen()) {
        DEBUG_LOG("[Game] Frame start");
        update();
        render();
    }
    DEBUG_LOG("[Game] Game loop ended");
}
