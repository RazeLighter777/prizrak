//
// Created by Justin.Suess on 12/26/2021.
//

#ifndef PRIZRAK_VOLUNTARYMOVEMENTHANDLER_H
#define PRIZRAK_VOLUNTARYMOVEMENTHANDLER_H


#include <list>
#include "EventHandler.h"
class Event;
class VoluntaryMovementHandler : public EventHandler {
public:
    VoluntaryMovementHandler(WorldSystemInterface* w) : EventHandler(w) {

    }
    void handleEntitiesWithEvent(std::list<Event *> events) override;
};


#endif //PRIZRAK_VOLUNTARYMOVEMENTHANDLER_H
