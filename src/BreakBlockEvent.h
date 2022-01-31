//
// Created by justin on 1/25/22.
//

#ifndef PRIZRAK_BREAKBLOCKEVENT_H
#define PRIZRAK_BREAKBLOCKEVENT_H

#include "MovementEvent.h"

class BreakBlockEvent : public Event {
public:
    DIRECTION direction;
    BreakBlockEvent(instanceId id, DIRECTION d, WorldSystemInterface* w) : Event(id), direction(d) {
        resource = stringHash("break");
        descriptionMapping.emplace_back(w->getEntityById(id).getComponentOfType<CreatureArchetypeId>().value()->rfid);
        descriptionMapping.emplace_back("a block");
        typeId = ID(BreakBlockEvent);
    }
};



#endif //PRIZRAK_BREAKBLOCKEVENT_H
