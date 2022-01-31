//
// Created by Justin.Suess on 10/28/2021.
//

#ifndef PRIZRAK_GAME_H
#define PRIZRAK_GAME_H

class World;
#include "libtcod/console.hpp"
#include "libtcod/libtcod.h"
class Game final {
private:
    static Game* instance;
public:
    static Game& getInstance() {
        if (instance == nullptr) {
            instance = new Game();
        }
        return *instance;
    }
    void loadStandardRawsAndInitializeComponents(World* w);
    [[noreturn]] void runGame();
};


#endif //PRIZRAK_GAME_H
