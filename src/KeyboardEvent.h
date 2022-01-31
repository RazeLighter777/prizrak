//
// Created by Justin.Suess on 12/26/2021.
//

#ifndef PRIZRAK_KEYBOARDEVENT_H
#define PRIZRAK_KEYBOARDEVENT_H


#include <libtcod/console_types.h>
#include "Event.h"

struct KeyboardEvent : public Event {

    KeyboardEvent(instanceId target, TCOD_key_t k) : Event(target), key(k) {
        typeId = stringHash(typeid(KeyboardEvent).name());
    }

    TCOD_key_t key;

};


#endif //PRIZRAK_KEYBOARDEVENT_H
