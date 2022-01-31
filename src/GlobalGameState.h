//
// Created by Justin.Suess on 12/26/2021.
//

#ifndef PRIZRAK_GLOBALGAMESTATE_H
#define PRIZRAK_GLOBALGAMESTATE_H


#include <libtcod/console.hpp>
#include <map>
#include "InputCollector.h"
#include "SoundEngine.h"
#include "typenames.h"

enum GAME_STATE {
    MAIN_MENU,
    HELP_MENU,
    IN_GAME,
    LEVEL_SELECT,
    QUIT,
};
class GlobalGameState;

class GlobalGameState {
private:
    std::map<uint64_t,std::string> resourcePack;
    static GlobalGameState* instance;
    TCODConsole& root;
    GAME_STATE state;
    InputCollector c;
    SoundEngine sound;
    instanceId playerInstanceId = 0;

    //default seed.
    uint64_t seed = 65537;
public:
    GlobalGameState();
    static GlobalGameState& getInstance();
    GAME_STATE getState() const;
    TCODConsole& getWindow();
    InputCollector& getInput();
    std::string getResourceByName(const char* ref);
    std::string getResourceById(uint64_t id);
    SoundEngine& getSound();
    instanceId getPlayerInstance();
    uint64_t getSeed();
    void exit();
    void setPlayerInstance(instanceId id);
};


#endif //PRIZRAK_GLOBALGAMESTATE_H
