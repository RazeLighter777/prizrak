//
// Created by justin on 1/23/22.
//

#ifndef PRIZRAK_EVENTLISTENINGCOMPONENT_H
#define PRIZRAK_EVENTLISTENINGCOMPONENT_H


#include "Component.h"

class EventListeningComponent : public Component {
public:

    std::list<Event*> currentEvents = {};

    json serializationFunction(void) override;

    static Component *deserialize(json& j, WorldSystemInterface *w, instanceId iid);

};


#endif //PRIZRAK_EVENTLISTENINGCOMPONENT_H
