//
// Created by justin on 12/27/21.
//

#ifndef PRIZRAK_BASICAISYSTEM_H
#define PRIZRAK_BASICAISYSTEM_H

#include <libtcod/fov.hpp>
#include "System.h"

class BasicAISystem : public System {
public:
    TCODMap* mapOfLoadedChunks;
    void process(WorldSystemInterface *w, std::vector<instanceId> entities) override;

    std::vector<componentTypeId> checkoutMatchingEntities() override;

    void generateAIMap(WorldSystemInterface *w);
};


#endif //PRIZRAK_BASICAISYSTEM_H
