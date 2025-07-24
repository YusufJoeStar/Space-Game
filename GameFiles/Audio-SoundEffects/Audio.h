#ifndef AUDIO_H
#define AUDIO_H

#include <SFML/Audio.hpp>
#include <string>
#include <vector>

class Audio {
public:
    Audio();
    ~Audio();

    bool loadMusic(const std::string& filepath);

    void addMusicFile(const std::string& filepath);
    bool loadRandomMusic();
    void clearMusicList();

    void play();
    void pause();
    void stop();
    void setLoop(bool loop);
    void setVolume(float vol);

private:
    sf::Music music;
    std::vector<std::string> musicFiles;
    std::string currentMusicFile;
};

#endif
