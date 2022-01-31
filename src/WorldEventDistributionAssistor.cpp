//
// Created by justin on 1/23/22.
//
#include <execution>
#include "WorldEventDistributionAssistor.h"
#include "Event.h"
#include "Entity.h"
#include "Position.h"
#include "EventListeningComponent.h"

void WorldEventDistributionAssistor::distributeEvents(const std::list<Event *> &events,
                                                      const std::list<instanceId>& listeners,
                                                      WorldSystemInterface* w) {
    std::for_each(std::execution::par_unseq, listeners.begin(), listeners.end(), [&w] (auto&& id) {
        w->getEntityById(id).template getComponentOfType<EventListeningComponent>().value()->currentEvents.clear();
    });
    std::for_each(std::execution::par_unseq, events.begin(), events.end(), [&w, &listeners] (auto&& e) {
        for (auto& ent : listeners) {
            w->getEntityById(ent).template getComponentOfType<EventListeningComponent>().value()->currentEvents.template emplace_back(e);
        }
    });
}
