#include "Audio-SoundEffects/Audio.h"
#include "Menu/game.h"
#include "Menu/Debug.h"
#include <iostream>

int main() {
    Audio bgm;

    bgm.addMusicFile("GameResources/Assets/Gwyn.flac");
    bgm.addMusicFile("GameResources/Assets/Prologue.flac");
    bgm.addMusicFile("GameResources/Assets/Pontiff.flac");
    bgm.addMusicFile("GameResources/Assets/SlaveKnight.flac");
    bgm.addMusicFile("GameResources/Assets/DragonSlayer.flac");
    bgm.addMusicFile("GameResources/Assets/Epilouge.flac");

    if (!bgm.loadRandomMusic()) {

    } else {
        bgm.setLoop(true);
        bgm.setVolume(70);
        bgm.play();
    }

    Game* game = new Game();
    game->run();
    delete game;

    DEBUG_LOG("[Main] Exiting main()");
    return 0;
}
