//
// Created by Justin.Suess on 12/26/2021.
//

#ifndef PRIZRAK_INPUTHANDLINGCOMPONENT_H
#define PRIZRAK_INPUTHANDLINGCOMPONENT_H


#include <libtcod/console_types.h>
#include "Component.h"

class InputHandlingComponent : public Component {
public:

    TCOD_key_t key;

    json serializationFunction(void) override;

    static Component *deserialize(json& j, WorldSystemInterface *w, instanceId iid);

};


#endif //PRIZRAK_INPUTHANDLINGCOMPONENT_H
