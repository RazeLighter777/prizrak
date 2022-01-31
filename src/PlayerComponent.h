//
// Created by Justin.Suess on 12/26/2021.
//

#ifndef PRIZRAK_PLAYERCOMPONENT_H
#define PRIZRAK_PLAYERCOMPONENT_H


#include "Component.h"

class PlayerComponent : public Component {

public:
    json serializationFunction(void) override;

    static Component *deserialize(json& j, WorldSystemInterface *w, instanceId iid);

};


#endif //PRIZRAK_PLAYERCOMPONENT_H
