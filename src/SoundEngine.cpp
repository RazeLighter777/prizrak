//
// Created by Justin.Suess on 12/26/2021.
//

#include <SFML/Audio.hpp>
#include <filesystem>
#include <iostream>
#include "SoundEngine.h"
#include "hashes.h"

SoundEngine::SoundEngine(const char* dir) {
    s.setLoop(false);
    for (const auto &dirEntry : std::filesystem::recursive_directory_iterator(std::string(dir) + "/music/")) {
        if (dirEntry.is_regular_file()) {
            auto* m = new sf::Music();
            std::string p = dirEntry.path().string();
            m->openFromFile(p);
            std::cout << "MUSIC FNAME : " << dirEntry.path().filename().string() << std::endl;
            music.insert({stringHash(dirEntry.path().filename().string().c_str()),m} );
        }
    }
    for (const auto &dirEntry : std::filesystem::recursive_directory_iterator(std::string(dir) + "/sound/")) {
        if (dirEntry.is_regular_file()) {
            std::string p = dirEntry.path().string();
            auto* m = new sf::SoundBuffer();
            m->loadFromFile(p);
            sound.insert({stringHash(dirEntry.path().filename().string().c_str()),m} );
        }
    }
}

void SoundEngine::setMusic(uint32_t musicId) {
    if (playingSong) {
        music[playingSong]->stop();
    }
    music[musicId]->setLoop(true);
    playingSong = musicId;
    music[musicId]->play();

}

void SoundEngine::playSound(uint32_t soundId) {
    s.setBuffer(*sound[soundId]);
    s.play();
}

SoundEngine::~SoundEngine() {
    for (auto& m : music) {
        delete m.second;
    }
    for (auto& s : sound) {
        delete s.second;
    }
}
