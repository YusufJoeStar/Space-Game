#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window/Event.hpp>
#include <string>
#include <optional>
#include "Audio-SoundEffects/SoundEffects.h"
#include "Textures-Time/Texture.h"


enum class MenuState {
    MENU,
    TRANSITIONING_TO_GAME,
    TRANSITIONING_TO_EXIT,
    GAME,
    EXIT
};

class Menu {
private:
    sf::RenderWindow* window;
    sf::Vector2f windowSize;


    sf::Font* menuFont;
    sf::Text* titleText;
    sf::Text* playButtonText;
    sf::Text* exitButtonText;
    sf::Texture* backgroundTexture;
    sf::Sprite* backgroundSprite;


    sf::FloatRect playButtonBounds;
    sf::FloatRect exitButtonBounds;


    MenuState currentState;


    sf::Color normalColor;
    sf::Color hoverColor;
    sf::Color clickColor;


    Sound* menuSoundSystem;
    bool playButtonWasHovered;
    bool exitButtonWasHovered;
    bool soundSystemInitialized;


    Texture* transitionTexture;
    sf::Sprite* transitionSprite;
    bool isTransitioning;
    float transitionTimer;
    float transitionDuration;
    MenuState targetState;
    sf::Vector2u transitionFrameSize;
    bool transitionAnimationComplete;

    void setupMenuElements();
    void setupSoundEffects();
    void setupTransitionAnimation();
    void handleMouseHover(sf::Vector2f mousePos);
    bool isMouseOverButton(sf::Vector2f mousePos, sf::FloatRect buttonBounds);
    void startTransition(MenuState target);
    void updateTransition(float deltaTime);
    void renderTransition();

public:
    Menu(sf::RenderWindow* gameWindow, sf::Vector2f winSize);
    ~Menu();

    MenuState handleEvents();
    void update(float deltaTime = 0.016f);
    void render();

    MenuState getCurrentState() const;
    void resetToMenu();
};
