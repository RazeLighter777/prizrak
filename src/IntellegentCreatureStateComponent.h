//
// Created by Justin.Suess on 10/30/2021.
//

#ifndef PRIZRAK_INTELLEGENTCREATURESTATECOMPONENT_H
#define PRIZRAK_INTELLEGENTCREATURESTATECOMPONENT_H


#include "Component.h"

enum INTELLEGENT_CREATURE_STATE {
    NO_INTELLEGENT_ACT = 0,
    READING = 1,
    CONVERSING = 2,
};
struct IntellegentCreatureStateComponent : public Component {
    INTELLEGENT_CREATURE_STATE state;

    static Component *deserialize(json& j, WorldSystemInterface *w, instanceId iid);

    json serializationFunction(void) override;
};


#endif //PRIZRAK_INTELLEGENTCREATURESTATECOMPONENT_H
