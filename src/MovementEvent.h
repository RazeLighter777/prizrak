//
// Created by Justin.Suess on 12/26/2021.
//

#ifndef PRIZRAK_MOVEMENTEVENT_H
#define PRIZRAK_MOVEMENTEVENT_H

#include "Event.h"
#include "Entity.h"
#include "CreatureArchetypeId.h"
enum DIRECTION {
    N,
    S,
    E,
    W,
    NE,
    NW,
    SE,
    SW,
};
class MovementEvent : public Event {
public:
    DIRECTION direction;
    MovementEvent(instanceId id, DIRECTION d, WorldSystemInterface* w) : Event(id), direction(d) {
        resource = stringHash("movement");
        descriptionMapping.emplace_back(w->getEntityById(id).getComponentOfType<CreatureArchetypeId>().value()->rfid);
        descriptionMapping.emplace_back("somewhere");
        typeId = ID(MovementEvent);
    }
};


#endif //PRIZRAK_MOVEMENTEVENT_H
