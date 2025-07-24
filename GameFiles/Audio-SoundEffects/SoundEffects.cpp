#include "Audio-SoundEffects/SoundEffects.h"
#include "Menu/Debug.h"
#include <iostream>
#include <algorithm>

Sound::Sound() : globalVolume(100.0f) {
}

Sound::~Sound() {
    stopAllSounds();
      sounds.clear();
    soundBuffers.clear();
    oneShotSounds.clear();
    rotationSounds.clear();
}

void Sound::setupSeamlessRotation(SoundEffect effect, float audioDuration) {
    auto bufferIt = soundBuffers.find(effect);
    if (bufferIt == soundBuffers.end()) {
        return;
    }

    rotationSoundDuration = audioDuration;

    for (int i = 0; i < 3; i++) {
        auto sound = std::make_unique<sf::Sound>(*bufferIt->second);
        sound->setVolume(0.0f);
        rotationSounds.push_back(std::move(sound));
    }
}

void Sound::playSeamlessRotation(float targetVolume) {
    if (rotationSounds.empty()) return;

    if (rotationSoundTimer <= 0.0f) {
        currentRotationSound = (currentRotationSound + 1) % rotationSounds.size();
        rotationSounds[currentRotationSound]->setVolume(targetVolume);
        rotationSounds[currentRotationSound]->play();
        rotationSoundTimer = rotationSoundDuration * 0.8f;
    }
}

void Sound::stopSeamlessRotation() {
    for (auto& sound : rotationSounds) {
        sound->stop();
    }
    rotationSoundTimer = 0.0f;
}

void Sound::updateSeamlessRotation(float deltaTime, float targetVolume) {
    if (rotationSounds.empty()) return;

    rotationSoundTimer -= deltaTime;

    for (auto& sound : rotationSounds) {
        if (sound->getStatus() == sf::Sound::Status::Playing) {
            sound->setVolume(targetVolume);
        }
    }
}

bool Sound::loadSoundEffect(SoundEffect effect, const std::string& filepath) {
    std::string effectName = getSoundEffectName(effect);

    auto buffer = std::make_unique<sf::SoundBuffer>();

    if (!buffer->loadFromFile(filepath)) {
        return false;
    }

    auto sound = std::make_unique<sf::Sound>(*buffer);
    sound->setVolume(0.0f);

    soundBuffers[effect] = std::move(buffer);
    sounds[effect] = std::move(sound);

    soundStates[effect] = SoundState();
    soundStates[effect].currentVolume = 0.0f;
    soundStates[effect].targetVolume = 0.0f;

    if (effect == SoundEffect::PELLET_FIRE ||
        effect == SoundEffect::PELLET_IMPACT ||
        effect == SoundEffect::SHIELD_ACTIVATE ||
        effect == SoundEffect::SHIELD_DEACTIVATE ||
        effect == SoundEffect::SHIELD_HIT ||
        effect == SoundEffect::SHIP_ROCK_COLLISION ||
        effect == SoundEffect::MENU_HOVER ||
        effect == SoundEffect::MENU_CLICK ||
        effect == SoundEffect::MENU_BACK ||
        effect == SoundEffect::MENU_CONFIRM ||
        effect == SoundEffect::MENU_TRANSITION) {

        int instances = (effect == SoundEffect::PELLET_FIRE || effect == SoundEffect::MENU_HOVER ||
                        effect == SoundEffect::MENU_CLICK) ? 5 : 3;

        for (int i = 0; i < instances; i++) {
            auto oneShotSound = std::make_unique<sf::Sound>(*soundBuffers[effect]);
            oneShotSound->setVolume(75.0f);
            oneShotSounds[effect].push_back(std::move(oneShotSound));
        }
    }

    return true;
}

void Sound::playOneShotEffect(SoundEffect effect, float volume) {
    std::string effectName = getSoundEffectName(effect);

    sf::Sound* availableSound = getAvailableOneShotSound(effect);
    if (availableSound) {
        availableSound->setVolume(volume);
        availableSound->play();
    }
}

sf::Sound* Sound::getAvailableOneShotSound(SoundEffect effect) {
    auto oneShotIt = oneShotSounds.find(effect);
    if (oneShotIt == oneShotSounds.end()) {
        return nullptr;
    }

    for (auto& sound : oneShotIt->second) {
        if (sound->getStatus() != sf::Sound::Status::Playing) {
            return sound.get();
        }
    }

    return oneShotIt->second[0].get();
}

void Sound::startSoundEffect(SoundEffect effect, bool loop) {
    std::string effectName = getSoundEffectName(effect);

    auto soundIt = sounds.find(effect);
    if (soundIt == sounds.end()) {
        return;
    }

    sf::Sound* sound = soundIt->second.get();
    SoundState& state = soundStates[effect];

    if (!state.isPlaying) {
        sound->setVolume(0.0f);
        sound->setLooping(loop);
        sound->play();
        state.isPlaying = true;
        state.currentVolume = 0.0f;
    }
}

void Sound::playSoundEffect(SoundEffect effect, bool loop) {
    startSoundEffect(effect, loop);
}

void Sound::stopSoundEffect(SoundEffect effect) {
    std::string effectName = getSoundEffectName(effect);

    auto soundIt = sounds.find(effect);
    if (soundIt != sounds.end()) {
        soundIt->second->stop();
        soundStates[effect].isPlaying = false;
        soundStates[effect].currentVolume = 0.0f;
        soundStates[effect].targetVolume = 0.0f;
    }
}

void Sound::pauseSoundEffect(SoundEffect effect) {
    std::string effectName = getSoundEffectName(effect);

    auto soundIt = sounds.find(effect);
    if (soundIt != sounds.end()) {
        soundIt->second->pause();
        soundStates[effect].isPlaying = false;
    }
}

void Sound::setTargetVolume(SoundEffect effect, float targetVolume) {
    auto stateIt = soundStates.find(effect);
    if (stateIt != soundStates.end()) {
        stateIt->second.targetVolume = std::min(targetVolume, stateIt->second.maxVolume);

        std::string effectName = getSoundEffectName(effect);

        if (stateIt->second.targetVolume > 0.0f && !stateIt->second.isPlaying) {
            startSoundEffect(effect, true);
        }
    }
}

void Sound::setFadeSpeed(SoundEffect effect, float fadeSpeed) {
    auto stateIt = soundStates.find(effect);
    if (stateIt != soundStates.end()) {
        stateIt->second.fadeSpeed = fadeSpeed;
    }
}

void Sound::setMaxVolume(SoundEffect effect, float maxVolume) {
    auto stateIt = soundStates.find(effect);
    if (stateIt != soundStates.end()) {
        stateIt->second.maxVolume = maxVolume;
        std::string effectName = getSoundEffectName(effect);
    }
}

void Sound::setSoundEffectVolume(SoundEffect effect, float volume) {
    auto soundIt = sounds.find(effect);
    if (soundIt != sounds.end()) {
        soundIt->second->setVolume(volume);
        soundStates[effect].currentVolume = volume;
        soundStates[effect].targetVolume = volume;
    }
}

void Sound::setGlobalSoundVolume(float volume) {
    globalVolume = volume;

    for (auto& soundPair : sounds) {
        soundPair.second->setVolume(globalVolume);
    }
}

bool Sound::isSoundPlaying(SoundEffect effect) {
    auto soundIt = sounds.find(effect);
    if (soundIt != sounds.end()) {
        return soundIt->second->getStatus() == sf::Sound::Status::Playing;
    }
    return false;
}

void Sound::update(float deltaTime) {
    for (auto& [effect, state] : soundStates) {
        updateSoundVolume(effect, deltaTime);

        if (state.isPlaying && state.currentVolume <= 0.0f && state.targetVolume <= 0.0f) {
            auto soundIt = sounds.find(effect);
            if (soundIt != sounds.end()) {
                soundIt->second->stop();
                state.isPlaying = false;
            }
        }
    }
}

void Sound::stopAllSounds(bool fadeOut) {
    if (fadeOut) {
        for (auto& [effect, state] : soundStates) {
            state.targetVolume = 0.0f;
        }
    } else {
        for (auto& soundPair : sounds) {
            soundPair.second->stop();
        }
        for (auto& [effect, state] : soundStates) {
            state.isPlaying = false;
            state.currentVolume = 0.0f;
            state.targetVolume = 0.0f;
        }

        for (auto& [effect, soundVector] : oneShotSounds) {
            for (auto& sound : soundVector) {
                sound->stop();
            }
        }
    }
}

void Sound::fadeOutSound(SoundEffect effect) {
    auto stateIt = soundStates.find(effect);
    if (stateIt != soundStates.end()) {
        stateIt->second.targetVolume = 0.0f;
        std::string effectName = getSoundEffectName(effect);
    }
}

void Sound::updateSoundVolume(SoundEffect effect, float deltaTime) {
    auto stateIt = soundStates.find(effect);
    auto soundIt = sounds.find(effect);

    if (stateIt == soundStates.end() || soundIt == sounds.end()) {
        return;
    }

    SoundState& state = stateIt->second;
    sf::Sound* sound = soundIt->second.get();

    if (std::abs(state.currentVolume - state.targetVolume) > 0.1f) {
        float direction = (state.targetVolume > state.currentVolume) ? 1.0f : -1.0f;
        float volumeChange = direction * state.fadeSpeed * deltaTime;

        state.currentVolume += volumeChange;

        if ((direction > 0 && state.currentVolume >= state.targetVolume) ||
            (direction < 0 && state.currentVolume <= state.targetVolume)) {
            state.currentVolume = state.targetVolume;
        }

        sound->setVolume(state.currentVolume);
    }
}

std::string Sound::getSoundEffectName(SoundEffect effect) {
    switch (effect) {
        case SoundEffect::SHIP_THRUST_NORMAL:
            return "SHIP_THRUST_NORMAL";
        case SoundEffect::SHIP_THRUST_BOOST:
            return "SHIP_THRUST_BOOST";
        case SoundEffect::SHIP_ROTATION:
            return "SHIP_ROTATION";
        case SoundEffect::ENGINE_IDLE:
            return "ENGINE_IDLE";
        case SoundEffect::PELLET_FIRE:
            return "PELLET_FIRE";
        case SoundEffect::PELLET_IMPACT:
            return "PELLET_IMPACT";
        case SoundEffect::SHIELD_ACTIVATE:
            return "SHIELD_ACTIVATE";
        case SoundEffect::SHIELD_DEACTIVATE:
            return "SHIELD_DEACTIVATE";
        case SoundEffect::SHIELD_HUM:
            return "SHIELD_HUM";
        case SoundEffect::SHIELD_HIT:
            return "SHIELD_HIT";
        case SoundEffect::SHIP_ROCK_COLLISION:
            return "SHIP_ROCK_COLLISION";
        case SoundEffect::MENU_HOVER:
            return "MENU_HOVER";
        case SoundEffect::MENU_CLICK:
            return "MENU_CLICK";
        case SoundEffect::MENU_BACK:
            return "MENU_BACK";
        case SoundEffect::MENU_CONFIRM:
            return "MENU_CONFIRM";
        case SoundEffect::MENU_TRANSITION:
            return "MENU_TRANSITION";
        default:
            return "UNKNOWN_SOUND_EFFECT";
    }
}
