//
// Created by Justin.Suess on 12/26/2021.
//

#ifndef PRIZRAK_INPUTGATHERINGSYSTEM_H
#define PRIZRAK_INPUTGATHERINGSYSTEM_H


#include "System.h"

class InputGatheringSystem : public System {
public:
    void process(WorldSystemInterface *w, std::vector<instanceId> entities) override;

    std::vector<componentTypeId> checkoutMatchingEntities() override;

};


#endif //PRIZRAK_INPUTGATHERINGSYSTEM_H
