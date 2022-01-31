//
// Created by justin on 1/25/22.
//

#ifndef PRIZRAK_BLOCKBREAKEVENTHANDLER_H
#define PRIZRAK_BLOCKBREAKEVENTHANDLER_H

#include <list>
class WorldSystemInterface;
#include "EventHandler.h"

class Event;
class BlockBreakEventHandler : public EventHandler {
public:
    BlockBreakEventHandler(WorldSystemInterface* w) : EventHandler(w) {

    }
    void handleEntitiesWithEvent(std::list<Event *> events) override;
};


#endif //PRIZRAK_BLOCKBREAKEVENTHANDLER_H
