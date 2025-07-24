#ifndef SOUND_H
#define SOUND_H

#include <SFML/Audio.hpp>
#include <string>
#include <map>
#include <memory>

enum class SoundEffect {

    SHIP_THRUST_NORMAL,
    SHIP_THRUST_BOOST,
    SHIP_ROTATION,
    ENGINE_IDLE,
    PELLET_FIRE,
    PELLET_IMPACT,
    SHIELD_ACTIVATE,
    SHIELD_DEACTIVATE,
    SHIELD_HUM,
    SHIELD_HIT,
    SHIP_ROCK_COLLISION,


    MENU_HOVER,
    MENU_CLICK,
    MENU_BACK,
    MENU_CONFIRM,
    MENU_TRANSITION
};


struct SoundState {
    float currentVolume = 0.0f;
    float targetVolume = 0.0f;
    float fadeSpeed = 150.0f;
    float maxVolume = 100.0f;
    bool isPlaying = false;
};

class Sound {
public:
    Sound();
    ~Sound();

    void setupSeamlessRotation(SoundEffect effect, float audioDuration);
    void playSeamlessRotation(float targetVolume);
    void stopSeamlessRotation();
    void updateSeamlessRotation(float deltaTime, float targetVolume);


    bool loadSoundEffect(SoundEffect effect, const std::string& filepath);


    void startSoundEffect(SoundEffect effect, bool loop = false);


    void playOneShotEffect(SoundEffect effect, float volume = 100.0f);


    void playSoundEffect(SoundEffect effect, bool loop = false);
    void stopSoundEffect(SoundEffect effect);
    void pauseSoundEffect(SoundEffect effect);


    void setTargetVolume(SoundEffect effect, float targetVolume);
    void setFadeSpeed(SoundEffect effect, float fadeSpeed);
    void setMaxVolume(SoundEffect effect, float maxVolume);


    void setSoundEffectVolume(SoundEffect effect, float volume);
    void setGlobalSoundVolume(float volume);


    bool isSoundPlaying(SoundEffect effect);


    void update(float deltaTime);


    void stopAllSounds(bool fadeOut = false);


    void fadeOutSound(SoundEffect effect);

private:

    std::vector<std::unique_ptr<sf::Sound>> rotationSounds;
    int currentRotationSound = 0;
    float rotationSoundDuration = 3.0f;
    float rotationSoundTimer = 0.0f;


    std::map<SoundEffect, std::unique_ptr<sf::SoundBuffer>> soundBuffers;


    std::map<SoundEffect, std::unique_ptr<sf::Sound>> sounds;


    std::map<SoundEffect, std::vector<std::unique_ptr<sf::Sound>>> oneShotSounds;


    std::map<SoundEffect, SoundState> soundStates;

    float globalVolume;


    std::string getSoundEffectName(SoundEffect effect);


    void updateSoundVolume(SoundEffect effect, float deltaTime);


    sf::Sound* getAvailableOneShotSound(SoundEffect effect);
};

#endif
