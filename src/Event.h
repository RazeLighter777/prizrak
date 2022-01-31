//
// Created by Justin.Suess on 10/14/2021.
//

#ifndef PRIZRAK_EVENT_H
#define PRIZRAK_EVENT_H

#include <stdexcept>
#include <list>
#include "typenames.h"
#include "WorldSystemInterface.h"
#include "GlobalGameState.h"
#include "Wuid.h"
#include "Util.h"

struct Event {
    eventInstanceId eid = (eventInstanceId) generateRandomWuid();
    instanceId target;

    Event(instanceId t) : target(t) {
    }

    eventTypeId typeId;
    std::list<std::string> descriptionMapping;
    uint32_t resource = 0;
    unsigned short soundRadius = 3;
    unsigned short importance  = 0;
    unsigned short sightRadius = 10;
    position location;
    bool universal = false;

    template<typename T>
    T *getEventAsType();

    std::string getStringRepresentation() {
        return substituteResources(resource,descriptionMapping,GlobalGameState::getInstance());
    }

    virtual ~Event() {
        //do nothing
    }
};

template<typename T>
T *Event::getEventAsType() {
    if (stringHash(typeid(T).name()) == typeId) {
        return static_cast<T *>(this);
    }
    return nullptr;
}

#define REGISTER_EVENT(type)     type (instanceId target) : Event(target), {typeId = stringHash(typeid( type ).name());


#endif //PRIZRAK_EVENT_H
