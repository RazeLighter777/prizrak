//
// Created by justin on 12/27/21.
//

#include <iostream>
#include <execution>
#include <future>
#include "BasicAISystem.h"
#include "Util.h"
#include "BaseCreatureStateComponent.h"
#include "Entity.h"
#include "PlayerComponent.h"
#include "MovementEvent.h"
#include "World.h"
#include "BlockType.h"
#include "CreatureArchetypeId.h"

void BasicAISystem::process(WorldSystemInterface *w, std::vector<instanceId> entities) {
    generateAIMap(w);
    int c = 0;
    std::for_each(std::execution::par_unseq, entities.begin(), entities.end(), [&w] (auto&& e){
        //deincrement move counter
        //wander idly given random chance 1/4 per tick by default.
        auto entity = w->getEntityById(e);
        auto baseComponent = entity.template getComponentOfType<BaseCreatureStateComponent>().value();
        //auto baseCreatureStats  =w->getRaw(entity.getComponentOfType<CreatureArchetypeId>().value()->archetypeid);
        if (baseComponent->moveCounter > 0) {
            baseComponent->moveCounter--;
        }
        if (baseComponent->bloodLevel <=  0.) {
            w->markEntityForDeletion(e);
        }
        if (baseComponent->state == FIGHTING && !entity.template getComponentOfType<PlayerComponent>().has_value()) {
        }
        if (baseComponent->state == IDLE && !entity.template getComponentOfType<PlayerComponent>().has_value()) {
            //wander in a random direction
            if (randint(1,10) == 1) {
                switch (randint(1,4)) {
                    case 1:
                        w->queueEvent(new MovementEvent(e,N,w));
                        break;
                    case 2:
                        w->queueEvent(new MovementEvent(e,S,w));
                        break;
                    case 3:
                        w->queueEvent(new MovementEvent(e,E,w));
                        break;
                    default:
                        w->queueEvent(new MovementEvent(e,W,w));
                }
            }
            //check if you are aligned differently see and enemy.
            //if (baseCreatureStats[""])

        }

    });

}

void BasicAISystem::generateAIMap(WorldSystemInterface *w) {//build map of loaded chunks once per tick
    delete mapOfLoadedChunks;
    uint16_t xmin = UINT16_MAX;
    uint16_t ymax = 0;
    uint16_t xmax = 0;
    uint16_t ymin = UINT16_MAX;
    for (auto c : w->getListOfLoadedChunks()) {
        //find lowest x coordinate and highest y coordinate.
        if (std::get<0>(World::getPositionOfChunk(c)) < xmin) {
            xmin = std::get<0>(World::getPositionOfChunk(c));
        }
        if (std::get<0>(World::getPositionOfChunk(c)) < ymin) {
            ymin = std::get<1>(World::getPositionOfChunk(c));
        }
        if (std::get<1>(World::getPositionOfChunk(c)) > ymax) {
            ymax = std::get<1>(World::getPositionOfChunk(c));
        }
        if (std::get<1>(World::getPositionOfChunk(c)) > xmax) {
            xmax = std::get<0>(World::getPositionOfChunk(c));
        }
    }
    xmax+=CHUNK_X_SIZE;
    ymax+=CHUNK_Y_SIZE;
    auto sizex = xmax - xmin;
    auto sizey = ymax - ymin;
    if (sizex > MAX_PATHFINDING_SIZE) {
        sizex = MAX_PATHFINDING_SIZE;
    }
    if (sizey > MAX_PATHFINDING_SIZE) {
        sizey = MAX_PATHFINDING_SIZE;
    }
    mapOfLoadedChunks = new TCODMap(sizex, sizey);
    mapOfLoadedChunks->clear();
    auto v = w->getListOfLoadedChunks();
    std::for_each(std::execution::par_unseq, v.begin(), v.end(), [this, ymin, xmin, &w] (auto c) {
        for (int i = 0; i < CHUNK_X_SIZE; i++) {
            for (int j = 0; j < CHUNK_Y_SIZE; j++) {
                position abspos = {std::get<0>(World::getPositionOfChunk(c)) + i,std::get<1>(World::getPositionOfChunk(c)) + j};
                auto solid = w->getBlockProperties(w->getBlockTypeAtPosition(abspos))->isSolid();
                auto chunkrelx = std::get<0>(abspos) - xmin;
                auto chunkrely = std::get<1>(abspos) - ymin;
                mapOfLoadedChunks->setProperties(chunkrelx,chunkrely,!solid,!solid);
            }
        }
    });
}

std::vector<componentTypeId> BasicAISystem::checkoutMatchingEntities() {
    return std::vector<componentTypeId>({ID(BaseCreatureStateComponent)});
}
