//
// Created by justin on 10/18/21.
//

#ifndef PRIZRAK_COLLISIONPROPERTIES_H
#define PRIZRAK_COLLISIONPROPERTIES_H


#include "Component.h"
#include <bitset>

struct CollisionProperties : public Component {
    std::bitset<8> flags;
};


#endif //PRIZRAK_COLLISIONPROPERTIES_H
