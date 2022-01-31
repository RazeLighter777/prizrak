//
// Created by Justin.Suess on 12/26/2021.
//
#include <execution>
#include <mutex>
#include "CreatureArchetypeId.h"
#include "WorldSystemInterface.h"
#include "BaseCreatureStateComponent.h"
#include "VoluntaryMovementHandler.h"
#include "MovementEvent.h"
#include "Entity.h"
#include "Position.h"
#include "BaseCreatureStats.h"

void VoluntaryMovementHandler::handleEntitiesWithEvent(std::list<Event *> events) {
    std::for_each(
            std::execution::par_unseq,
            events.begin(),
            events.end(),
            [this](auto&& e)
            {
                std::mutex mtx;
                //do stuff with item
                //TODO: Compute speed stat properly.
                MovementEvent* m = static_cast<MovementEvent*>(e);
                //check if the entity wasn't unloaded in the previous tick.
                if (!world->entityWithIdExists(e->target)) {
                    return;
                }
                //check if the entity has the required components
                auto positionComponent = world->getEntityById(e->target).template getComponentOfType<Position>();
                auto creatureArchetypeComponent = world->getEntityById(e->target).template getComponentOfType<CreatureArchetypeId>();
                auto baseCreatureStateComponent = world->getEntityById(e->target).template getComponentOfType<BaseCreatureStateComponent>();
                if (creatureArchetypeComponent.has_value() && positionComponent.has_value() && baseCreatureStateComponent.has_value() && baseCreatureStateComponent.value()->moveCounter == 0 ) {
                    position poriginal = positionComponent.value()->get();
                    position p = positionComponent.value()->get();
                    if (m->direction == N) {
                        p = {std::get<0>(p), std::get<1>(p) - 1};
                    } else if (m->direction == S) {
                        p = {std::get<0>(p), std::get<1>(p)  + 1};
                    } else if (m->direction == E) {
                        p = {std::get<0>(p) + 1, std::get<1>(p)};
                    } else if (m->direction == W) {
                        p = {std::get<0>(p) - 1, std::get<1>(p)};
                    }
                    //TODO: Compute collision properly
                    if (! world->getBlockProperties(world->getBlockTypeAtPosition(p))->isSolid() || world->getBlockProperties(world->getBlockTypeAtPosition(poriginal))->isSolid()) {
                        //baseCreatureStateComponent.value()->moveCounter += world->getRaw(creatureArchetypeComponent.value()->archetypeid)["baseSpeed"].template get<int>();
                        baseCreatureStateComponent.value()->moveCounter += 5;
                        mtx.lock();
                        positionComponent.value()->operator=(p);
                        mtx.unlock();
                    }
                }

            });
}
