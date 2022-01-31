//
// Created by Justin.Suess on 12/26/2021.
//

#include "InputCollector.h"

InputCollector::InputCollector(TCODConsole &c): con(c) {

}

TCOD_key_t InputCollector::blockingKeyboardInput() {
    return this->con.waitForKeypress(true);
}

TCOD_key_t InputCollector::nonBlockingKeyboardInput() {
    return this->con.checkForKeypress();
}
