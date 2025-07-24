#include "Audio-SoundEffects/Audio.h"
#include "Physics-Colors/Randomize.h"
#include <iostream>
#include "Menu/Debug.h"

Audio::Audio() {
    ;
}

Audio::~Audio() {
    music.stop();
}

bool Audio::loadMusic(const std::string& filepath) {
    if (!music.openFromFile(filepath)) {
        DEBUG_LOG("[Audio][ERROR] Failed to open file: " << filepath);
        return false;
    }

    currentMusicFile = filepath;
    DEBUG_LOG("[Audio] Successfully loaded music: " << filepath);
    return true;
}

void Audio::addMusicFile(const std::string& filepath) {
    musicFiles.push_back(filepath);
}

bool Audio::loadRandomMusic() {
    if (musicFiles.empty()) {
        DEBUG_LOG("[Audio][ERROR] No music files in playlist!");
        return false;
    }

    DEBUG_LOG("[Audio] Selecting random music from " << musicFiles.size() << " files");

    Randomize randomizer;
    randomizer.randomizeInt(0, musicFiles.size() - 1);
    int randomIndex = randomizer.randomI();

    std::string selectedFile = musicFiles[randomIndex];
    DEBUG_LOG("[Audio] Randomly selected: " << selectedFile << " (index " << randomIndex << ")");

    return loadMusic(selectedFile);
}

void Audio::clearMusicList() {
    DEBUG_LOG("[Audio] Clearing music playlist");
    musicFiles.clear();
}

void Audio::play() {
    music.play();
}

void Audio::pause() {
    DEBUG_LOG("[Audio] Pausing music");
    music.pause();
}

void Audio::stop() {
    DEBUG_LOG("[Audio] Stopping music");
    music.stop();
}

void Audio::setVolume(float vol) {
    music.setVolume(vol);
}

void Audio::setLoop(bool loop) {
    music.setLooping(loop);
}
