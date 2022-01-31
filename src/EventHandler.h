//
// Created by Justin.Suess on 10/14/2021.
//

#ifndef PRIZRAK_EVENTHANDLER_H
#define PRIZRAK_EVENTHANDLER_H

class Event;
class EventHandler {
protected:
    WorldSystemInterface *world;
public:
    explicit EventHandler(WorldSystemInterface *w) : world(w) {

    }

    EventHandler() {}

    virtual void handleEntitiesWithEvent(std::list<Event *> events) = 0;
};


#endif //PRIZRAK_EVENTHANDLER_H
