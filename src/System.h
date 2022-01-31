//
// Created by Justin.Suess on 10/5/2021.
//

#ifndef PRIZRAK_SYSTEM_H
#define PRIZRAK_SYSTEM_H

#include <atomic>
#include <memory>
#include <vector>
#include "typenames.h"
#include "WorldSystemInterface.h"

struct Entity;

class System {
public:
    virtual void process(WorldSystemInterface *w, std::vector<instanceId> entities) = 0;

    virtual std::vector<componentTypeId> checkoutMatchingEntities() = 0;
};


#endif //PRIZRAK_SYSTEM_H
