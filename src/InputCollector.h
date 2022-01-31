//
// Created by Justin.Suess on 12/26/2021.
//

#ifndef PRIZRAK_INPUTCOLLECTOR_H
#define PRIZRAK_INPUTCOLLECTOR_H


#include <libtcod/console.hpp>

class InputCollector {
private:
    TCODConsole& con;
public:
     InputCollector(TCODConsole& con);

     TCOD_key_t blockingKeyboardInput();
     TCOD_key_t nonBlockingKeyboardInput();
};


#endif //PRIZRAK_INPUTCOLLECTOR_H
