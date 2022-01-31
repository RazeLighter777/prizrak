//
// Created by Justin.Suess on 12/26/2021.
//

#include <fstream>
#include "json.hpp"
#include "GlobalGameState.h"
#include "hashes.h"

GlobalGameState &GlobalGameState::getInstance() {
    if (instance == nullptr) {
        instance = new GlobalGameState();
    }
    return *instance;
}

GAME_STATE GlobalGameState::getState() const {
    return state;
}

TCODConsole &GlobalGameState::getWindow() {
    return root;
}

GlobalGameState::GlobalGameState() : root(*TCODConsole::root), state(MAIN_MENU), c(root), sound("./sound") {
    std::ifstream ifs("resources.json");
    nlohmann::json jf = nlohmann::json::parse(ifs);
    for (auto it : jf.items()) {
        resourcePack.insert({stringHash(it.key().c_str()), it.value()});
    }

}

InputCollector &GlobalGameState::getInput() {
    return c;
}

GlobalGameState* GlobalGameState::instance = nullptr;

SoundEngine &GlobalGameState::getSound() {
    return sound;
}

instanceId GlobalGameState::getPlayerInstance() {
    return playerInstanceId;
}

void GlobalGameState::setPlayerInstance(instanceId id) {
    playerInstanceId = id;
}


void GlobalGameState::exit() {
    state = QUIT;
}

uint64_t GlobalGameState::getSeed() {
    return seed;
}

std::string GlobalGameState::getResourceByName(const char *ref) {
    return resourcePack[stringHash(ref)];
}

std::string GlobalGameState::getResourceById(uint64_t id) {
    return resourcePack[id];
}

