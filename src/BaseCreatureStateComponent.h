//
// Created by Justin.Suess on 10/29/2021.
//

#ifndef PRIZRAK_BASECREATURESTATECOMPONENT_H
#define PRIZRAK_BASECREATURESTATECOMPONENT_H


#include "Component.h"
//States all creatures may be in
enum BASE_CREATURE_STATE {
    FIGHTING = 0,
    SLEEPING = 1,
    EATING = 2,
    IDLE = 3,
    REPRODUCING = 4,
};


struct BaseCreatureStateComponent : public Component {
    double bloodLevel;
    uint16_t stunCounter;
    uint16_t age;
    uint16_t calories;
    uint16_t moveCounter;
    BASE_CREATURE_STATE state;
    json serializationFunction(void) override;
    static Component *deserialize(json& j, WorldSystemInterface *w, instanceId iid);

};


#endif //PRIZRAK_BASECREATURESTATECOMPONENT_H
