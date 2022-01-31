//
// Created by Justin.Suess on 10/29/2021.
//

#ifndef PRIZRAK_APPEARANCE_H
#define PRIZRAK_APPEARANCE_H

#include "typenames.h"
#include "libtcod/color.hpp"
#include <list>
enum ANIMATION {
    BLINK,
    STATIC,
    CYCLE,
};
enum LAYER {
    GROUND,
    ON_GROUND,
    ABOVE_GROUND,
};
struct Appearance {
    //which layer the sprite is rendered to.
    LAYER layer;
    //how the game scrolls through each sprite
    ANIMATION animation;
    //for this version of the game, spriteId is just the ascii code.
    std::list<std::tuple<spriteId, TCODColor, TCODColor>> spriteIds;
};


#define SL_APPEARANCE(raw) raw =

#endif //PRIZRAK_APPEARANCE_H
