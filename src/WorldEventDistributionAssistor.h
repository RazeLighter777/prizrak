//
// Created by justin on 1/23/22.
//

#ifndef PRIZRAK_WORLDEVENTDISTRIBUTIONASSISTOR_H
#define PRIZRAK_WORLDEVENTDISTRIBUTIONASSISTOR_H


#include "Event.h"

class WorldEventDistributionAssistor {
public:
    static void distributeEvents(const std::list<Event*>& events, const std::list<instanceId>& listeners, WorldSystemInterface* w);
};


#endif //PRIZRAK_WORLDEVENTDISTRIBUTIONASSISTOR_H
