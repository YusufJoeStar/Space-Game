#include "Menu/Menu.h"
#include "Menu/Debug.h"
#include <optional>
#include <algorithm>
#include <SFML/Window/Event.hpp>

Menu::Menu(sf::RenderWindow* gameWindow, sf::Vector2f winSize)
    : window(gameWindow), windowSize(winSize), currentState(MenuState::MENU),
      playButtonWasHovered(false), exitButtonWasHovered(false), soundSystemInitialized(false),
      isTransitioning(false), transitionTimer(0.0f), transitionDuration(2.0f),
      targetState(MenuState::MENU), transitionAnimationComplete(false) {

    DEBUG_LOG("[Menu] Constructor start");

    normalColor = sf::Color::White;
    hoverColor = sf::Color::Yellow;
    clickColor = sf::Color::Red;

    menuFont = nullptr;
    titleText = nullptr;
    playButtonText = nullptr;
    exitButtonText = nullptr;
    backgroundTexture = nullptr;
    backgroundSprite = nullptr;
    menuSoundSystem = nullptr;
    transitionTexture = nullptr;
    transitionSprite = nullptr;

    setupMenuElements();
    setupSoundEffects();
    setupTransitionAnimation();

    DEBUG_LOG("[Menu] Constructor complete");
}

Menu::~Menu() {
    DEBUG_LOG("[Menu] Destructor");
    delete titleText;
    delete playButtonText;
    delete exitButtonText;
    delete backgroundSprite;
    delete backgroundTexture;
    delete menuFont;
    delete menuSoundSystem;
    delete transitionSprite;
    delete transitionTexture;
}

void Menu::setupSoundEffects() {
    DEBUG_LOG("[Menu] Setting up sound effects");

    menuSoundSystem = new Sound();

    bool hoverLoaded = menuSoundSystem->loadSoundEffect(SoundEffect::MENU_HOVER, "GameResources/Assets/MenuHover.ogg");
    if (!hoverLoaded) {
        hoverLoaded = menuSoundSystem->loadSoundEffect(SoundEffect::MENU_HOVER, "GameResources/Assets/Sounds/MenuHover.ogg");
        if (!hoverLoaded) {
            hoverLoaded = menuSoundSystem->loadSoundEffect(SoundEffect::MENU_HOVER, "GameResources/Assets/MenuHover.ogg");
        }
        DEBUG_LOG("[Menu] Attempting to load hover sound from alternative paths");
    }

    bool clickLoaded = menuSoundSystem->loadSoundEffect(SoundEffect::MENU_CLICK, "GameResources/Assets/MenuClick.ogg");
    if (!clickLoaded) {
        clickLoaded = menuSoundSystem->loadSoundEffect(SoundEffect::MENU_CLICK, "GameResources/Assets/MenuClick.ogg");
        if (!clickLoaded) {
            clickLoaded = menuSoundSystem->loadSoundEffect(SoundEffect::MENU_CLICK, "GameResources/Assets/MenuClick.ogg");
        }
        DEBUG_LOG("[Menu] Attempting to load click sound from alternative paths");
    }

    menuSoundSystem->loadSoundEffect(SoundEffect::MENU_TRANSITION, "GameResources/Sounds/menu_transition.wav");
    menuSoundSystem->loadSoundEffect(SoundEffect::MENU_CONFIRM, "GameResources/Sounds/menu_confirm.wav");

    menuSoundSystem->setMaxVolume(SoundEffect::MENU_HOVER, 50.0f);
    menuSoundSystem->setMaxVolume(SoundEffect::MENU_CLICK, 70.0f);
    menuSoundSystem->setMaxVolume(SoundEffect::MENU_TRANSITION, 60.0f);
    menuSoundSystem->setMaxVolume(SoundEffect::MENU_CONFIRM, 80.0f);

    soundSystemInitialized = (hoverLoaded || clickLoaded);

    if (soundSystemInitialized) {
        DEBUG_LOG("[Menu] Menu sound effects setup complete");
    } else {
        DEBUG_LOG("[Menu][WARNING] Failed to load menu sound effects - menu will be silent");
    }
}

void Menu::setupMenuElements() {
    DEBUG_LOG("[Menu] Setting up menu elements");

    menuFont = new sf::Font();
    if (!menuFont->openFromFile("GameResources/Fonts/ariblk.ttf")) {
        DEBUG_LOG("[Menu][ERROR] Failed to load font");
        return;
    }

    backgroundTexture = new sf::Texture();
    if (!backgroundTexture->loadFromFile("GameResources/Resources/blue-preview.png")) {
        if (!backgroundTexture->loadFromFile("GameResources/Resources/BackgroundSpace3.png")) {
            if (!backgroundTexture->loadFromFile("GameResources/Resources/SpaceBackground2.png")) {
                backgroundTexture->loadFromFile("GameResources/Resources/SpaceBackround.png");
            }
        }
    }

    backgroundSprite = new sf::Sprite(*backgroundTexture);
    sf::Vector2u textureSize = backgroundTexture->getSize();
    float scaleX = windowSize.x / textureSize.x;
    float scaleY = windowSize.y / textureSize.y;
    float scale = (scaleX > scaleY) ? scaleX : scaleY;
    backgroundSprite->setScale({scale, scale});

    sf::FloatRect bgBounds = backgroundSprite->getGlobalBounds();
    backgroundSprite->setPosition({(windowSize.x - bgBounds.size.x) / 2.0f, (windowSize.y - bgBounds.size.y) / 2.0f});

    titleText = new sf::Text(*menuFont);
    titleText->setString("SPACE GAME");
    titleText->setCharacterSize(72);
    titleText->setFillColor(sf::Color::White);
    titleText->setStyle(sf::Text::Bold);

    sf::FloatRect titleBounds = titleText->getLocalBounds();
    titleText->setPosition({(windowSize.x - titleBounds.size.x) / 2.0f, windowSize.y * 0.2f});

    playButtonText = new sf::Text(*menuFont);
    playButtonText->setString("PLAY");
    playButtonText->setCharacterSize(48);
    playButtonText->setFillColor(normalColor);
    playButtonText->setStyle(sf::Text::Bold);

    sf::FloatRect playBounds = playButtonText->getLocalBounds();
    playButtonText->setPosition({(windowSize.x - playBounds.size.x) / 2.0f, windowSize.y * 0.5f});

    playButtonBounds.position.x = playButtonText->getPosition().x - 20;
    playButtonBounds.position.y = playButtonText->getPosition().y - 10;
    playButtonBounds.size.x = playBounds.size.x + 40;
    playButtonBounds.size.y = playBounds.size.y + 20;

    exitButtonText = new sf::Text(*menuFont);
    exitButtonText->setString("EXIT");
    exitButtonText->setCharacterSize(48);
    exitButtonText->setFillColor(normalColor);
    exitButtonText->setStyle(sf::Text::Bold);

    sf::FloatRect exitBounds = exitButtonText->getLocalBounds();
    exitButtonText->setPosition({(windowSize.x - exitBounds.size.x) / 2.0f, windowSize.y * 0.7f});

    exitButtonBounds.position.x = exitButtonText->getPosition().x - 20;
    exitButtonBounds.position.y = exitButtonText->getPosition().y - 10;
    exitButtonBounds.size.x = exitBounds.size.x + 40;
    exitButtonBounds.size.y = exitBounds.size.y + 20;

    DEBUG_LOG("[Menu] Menu elements setup complete");
}

void Menu::setupTransitionAnimation() {
    DEBUG_LOG("[Menu] Setting up transition animation");

    transitionFrameSize = {800, 600};

    try {
        transitionTexture = new Texture("GameResources/Assets/TransitionBackground.png", transitionFrameSize);

        if (transitionTexture->getSize().x == 0 || transitionTexture->getSize().y == 0) {
            delete transitionTexture;
            transitionTexture = new Texture("GameResources/Assets/menu_transition.png", transitionFrameSize);

            if (transitionTexture->getSize().x == 0 || transitionTexture->getSize().y == 0) {
                delete transitionTexture;
                transitionTexture = new Texture("Resources/Assets/transition.png", transitionFrameSize);

                if (transitionTexture->getSize().x == 0 || transitionTexture->getSize().y == 0) {
                    delete transitionTexture;
                    transitionTexture = nullptr;
                }
            }
        }
    } catch (...) {
        transitionTexture = nullptr;
    }

    if (transitionTexture) {
        transitionSprite = new sf::Sprite(*transitionTexture);

        sf::Vector2u textureSize = transitionTexture->getSize();
        float scaleX = windowSize.x / transitionFrameSize.x;
        float scaleY = windowSize.y / transitionFrameSize.y;
        float scale = std::max(scaleX, scaleY);
        transitionSprite->setScale({scale, scale});

        sf::FloatRect transBounds = transitionSprite->getGlobalBounds();
        transitionSprite->setPosition({
            (windowSize.x - transBounds.size.x) / 2.0f,
            (windowSize.y - transBounds.size.y) / 2.0f
        });

        transitionTexture->setframetimerMax(0.1f);

        DEBUG_LOG("[Menu] Transition animation setup complete");
    } else {
        DEBUG_LOG("[Menu][WARNING] Failed to load transition animation - using simple fade");
        transitionSprite = nullptr;
    }
}

MenuState Menu::handleEvents() {
    while (const std::optional event = window->pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            DEBUG_LOG("[Menu] Window closed event");
            return MenuState::EXIT;
        }

        if (const auto* keypressed = event->getIf<sf::Event::KeyPressed>()) {
            if (keypressed->scancode == sf::Keyboard::Scancode::Escape) {
                DEBUG_LOG("[Menu] Escape key pressed");
                return MenuState::EXIT;
            }
        }

        if (const auto* mousePressed = event->getIf<sf::Event::MouseButtonPressed>()) {
            if (mousePressed->button == sf::Mouse::Button::Left) {
                sf::Vector2f mousePos = sf::Vector2f(sf::Mouse::getPosition(*window));

                if (isMouseOverButton(mousePos, playButtonBounds)) {
                    DEBUG_LOG("[Menu] Play button clicked");
                    playButtonText->setFillColor(clickColor);

                    if (soundSystemInitialized && menuSoundSystem) {
                        menuSoundSystem->playOneShotEffect(SoundEffect::MENU_CLICK, 70.0f);
                    }

                    startTransition(MenuState::GAME);
                    return currentState;
                }

                if (isMouseOverButton(mousePos, exitButtonBounds)) {
                    DEBUG_LOG("[Menu] Exit button clicked");
                    exitButtonText->setFillColor(clickColor);

                    if (soundSystemInitialized && menuSoundSystem) {
                        menuSoundSystem->playOneShotEffect(SoundEffect::MENU_CLICK, 70.0f);
                    }

                    startTransition(MenuState::EXIT);
                    return currentState;
                }
            }
        }
    }

    return currentState;
}

void Menu::update(float deltaTime) {
    if (isTransitioning) {
        updateTransition(deltaTime);
        return;
    }

    sf::Vector2f mousePos = sf::Vector2f(sf::Mouse::getPosition(*window));
    handleMouseHover(mousePos);

    if (soundSystemInitialized && menuSoundSystem) {
        menuSoundSystem->update(deltaTime);
    }
}

void Menu::handleMouseHover(sf::Vector2f mousePos) {
    bool playCurrentlyHovered = isMouseOverButton(mousePos, playButtonBounds);
    bool exitCurrentlyHovered = isMouseOverButton(mousePos, exitButtonBounds);

    playButtonText->setFillColor(normalColor);
    exitButtonText->setFillColor(normalColor);

    if (playCurrentlyHovered) {
        playButtonText->setFillColor(hoverColor);

        if (!playButtonWasHovered && soundSystemInitialized && menuSoundSystem) {
            menuSoundSystem->playOneShotEffect(SoundEffect::MENU_HOVER, 50.0f);
            DEBUG_LOG("[Menu] Play button hovered - sound played");
        }
    }

    if (exitCurrentlyHovered) {
        exitButtonText->setFillColor(hoverColor);

        if (!exitButtonWasHovered && soundSystemInitialized && menuSoundSystem) {
            menuSoundSystem->playOneShotEffect(SoundEffect::MENU_HOVER, 50.0f);
            DEBUG_LOG("[Menu] Exit button hovered - sound played");
        }
    }

    playButtonWasHovered = playCurrentlyHovered;
    exitButtonWasHovered = exitCurrentlyHovered;
}

bool Menu::isMouseOverButton(sf::Vector2f mousePos, sf::FloatRect buttonBounds) {
    return buttonBounds.contains(mousePos);
}

void Menu::render() {
    window->clear();

    if (backgroundSprite) {
        window->draw(*backgroundSprite);
    }

    if (!isTransitioning || transitionTimer < transitionDuration * 0.3f) {
        if (titleText) window->draw(*titleText);
        if (playButtonText) window->draw(*playButtonText);
        if (exitButtonText) window->draw(*exitButtonText);
    }

    if (isTransitioning) {
        renderTransition();
    }

    window->display();
}

MenuState Menu::getCurrentState() const {
    return currentState;
}

void Menu::resetToMenu() {
    currentState = MenuState::MENU;
    playButtonWasHovered = false;
    exitButtonWasHovered = false;
    isTransitioning = false;
    transitionTimer = 0.0f;
    transitionAnimationComplete = false;
    DEBUG_LOG("[Menu] Reset to menu state");
}

void Menu::startTransition(MenuState target) {
    DEBUG_LOG("[Menu] Starting transition to target state");

    isTransitioning = true;
    targetState = target;
    transitionTimer = 0.0f;
    transitionAnimationComplete = false;

    if (target == MenuState::GAME) {
        currentState = MenuState::TRANSITIONING_TO_GAME;
    } else if (target == MenuState::EXIT) {
        currentState = MenuState::TRANSITIONING_TO_EXIT;
    }

    if (soundSystemInitialized && menuSoundSystem) {
        menuSoundSystem->playOneShotEffect(SoundEffect::MENU_TRANSITION, 60.0f);
    }

    if (transitionTexture) {
        transitionTexture->setframetimer(0.0f);
    }
}

void Menu::updateTransition(float deltaTime) {
    transitionTimer += deltaTime;

    if (transitionTexture && transitionSprite) {
        transitionTexture->animate(deltaTime);

        sf::IntRect frameRect(
            sf::Vector2i(transitionTexture->get_texpos().x, transitionTexture->get_texpos().y),
            sf::Vector2i(static_cast<int>(transitionFrameSize.x), static_cast<int>(transitionFrameSize.y))
        );
        transitionSprite->setTextureRect(frameRect);
    }

    if (transitionTimer >= transitionDuration) {
        isTransitioning = false;
        currentState = targetState;
        transitionAnimationComplete = true;

        DEBUG_LOG("[Menu] Transition complete - new state: " << (int)currentState);
    }
}

void Menu::renderTransition() {
    if (transitionSprite && transitionTexture) {
        float progress = transitionTimer / transitionDuration;
        unsigned char alphaValue = static_cast<unsigned char>(255 * progress);
        transitionSprite->setColor(sf::Color(255, 255, 255, alphaValue));

        window->draw(*transitionSprite);
    } else {
        sf::RectangleShape fadeOverlay(windowSize);
        float progress = transitionTimer / transitionDuration;
        unsigned char alphaValue = static_cast<unsigned char>(255 * progress);
        fadeOverlay.setFillColor(sf::Color(0, 0, 0, alphaValue));
        window->draw(fadeOverlay);
    }
}
