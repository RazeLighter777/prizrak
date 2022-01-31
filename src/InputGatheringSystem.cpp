//
// Created by Justin.Suess on 12/26/2021.
//

#include "InputGatheringSystem.h"
#include "PlayerComponent.h"
#include "GlobalGameState.h"
#include "Entity.h"
#include "MovementEvent.h"
#include "BaseCreatureStateComponent.h"
#include "CreatureArchetypeId.h"
#include "Position.h"
#include "BreakBlockEvent.h"

void InputGatheringSystem::process(WorldSystemInterface *w, std::vector<instanceId> entities) {
    for (auto e : entities) {
        auto baseStateComp = w->getEntityById(e).getComponentOfType<BaseCreatureStateComponent>();
        auto position = w->getEntityById(e).getComponentOfType<Position>();

        //Check to make sure the move counter is zero when prompting for input
        if (baseStateComp.has_value() && baseStateComp.value()->moveCounter == 0) {
            //TODO: Code key bindings based on GlobalGameState (settings)
            bool reloop = true;
            while (reloop) {
                reloop = false;
                auto key = GlobalGameState::getInstance().getInput().blockingKeyboardInput();
                if (key.c == 'h') {
                    w->queueEvent(new MovementEvent(e,DIRECTION::W,w));
                } else if (key.c == 'l') {
                    w->queueEvent(new MovementEvent(e, DIRECTION::E,w));
                } else if (key.c == 'j') {
                    w->queueEvent(new MovementEvent(e, DIRECTION::S,w));
                } else if (key.c == 'k') {
                    w->queueEvent(new MovementEvent(e, DIRECTION::N,w));
                } else if (key.c == 's') {
                    w->saveWorld();
                    for (int i = 0; i < 5; i++) {
                        CreatureArchetypeId::createCreatureInWorld(w, generateRandomWuid(),"creature/lizard",{32001,32000});
                    }
                } else if (key.vk == TCODK_ESCAPE) {
                    GlobalGameState::getInstance().exit();
                } else if (key.c == '.') {
                    baseStateComp.value()->moveCounter+=5;
                }
                else if (key.vk == TCODK_SHIFT) {
                    auto key = GlobalGameState::getInstance().getInput().blockingKeyboardInput();
                    if (key.c == 'h') {
                        w->queueEvent(new BreakBlockEvent(e,DIRECTION::W,w));
                    } else if (key.c == 'l') {
                        w->queueEvent(new BreakBlockEvent(e, DIRECTION::E,w));
                    } else if (key.c == 'j') {
                        w->queueEvent(new BreakBlockEvent(e, DIRECTION::S,w));
                    } else if (key.c == 'k') {
                        w->queueEvent(new BreakBlockEvent(e, DIRECTION::N,w));
                    } else {
                        reloop = true;
                    }
                } else {
                    reloop = true;
                }
            }

        }
    }
}

std::vector<componentTypeId> InputGatheringSystem::checkoutMatchingEntities() {
    return std::vector<componentTypeId>({stringHash(typeid(PlayerComponent).name())});
}
