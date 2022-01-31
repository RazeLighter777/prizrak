//
// Created by Justin.Suess on 12/26/2021.
//

#ifndef PRIZRAK_SOUNDENGINE_H
#define PRIZRAK_SOUNDENGINE_H
#include <SFML/Audio.hpp>
#include <map>
class SoundEngine {
private:
    uint32_t playingSong = 0;
    std::map<uint32_t, sf::Music*> music;
    std::map<uint32_t, sf::SoundBuffer*> sound;
    sf::Sound s;
public:
    SoundEngine(const char* dir);
    ~SoundEngine();

    void playSound(uint32_t soundId);

    void setMusic(uint32_t musicId);
};


#endif //PRIZRAK_SOUNDENGINE_H
