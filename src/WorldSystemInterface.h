//
// Created by Justin.Suess on 10/5/2021.
//

#ifndef PRIZRAK_WORLDSYSTEMINTERFACE_H
#define PRIZRAK_WORLDSYSTEMINTERFACE_H

#include "hashes.h"
#include "Event.h"
#include "BlockType.h"
struct Component;
struct Entity;
struct Event;
class Biome;
class WorldSystemInterface {
private:
    friend class Position;

    virtual void entityPositionUpdate(instanceId id, position p, bool loadChunks) = 0;

    virtual void entityPositionRemoval(instanceId id) = 0;

public:
    virtual Entity &getEntityById(instanceId iid) = 0;

    virtual bool componentTypeIdMatches(componentTypeId, componentInstanceId) = 0;

    virtual void markEntityForDeletion(instanceId id) = 0;

    virtual instanceId createEntityFromComponents(std::vector<json> j) = 0;

    virtual void markComponentForDeletion(componentInstanceId id) = 0;

    virtual bool entityWithIdExists(instanceId id) const = 0;

    virtual void queueEvent(Event *event) = 0;

    virtual void loadEntityExplicity(instanceId iid) = 0;

    virtual const json& getRaw(uint16_t id) = 0;

    virtual Biome* getBiomeAtPosition(position pos) = 0;

    virtual blockTypeId getBlockTypeFromRaws(std::string) = 0;

    virtual void createBlockAtPositionById(blockTypeId, position pos) = 0;

    virtual void deleteBlockAtPosition(position pos) = 0;

    virtual unsigned int getNumberOfLoadedEntities() const  = 0;

    virtual blockTypeId getBlockTypeAtPosition(position pos) = 0;

    virtual BlockType *getBlockProperties(blockTypeId id) = 0;

    virtual bool chunkLoaded(chunkId id) const = 0;

    virtual void unloadChunk(chunkId id) = 0;

    virtual void addComponentToEntity(Component *component, instanceId id) = 0;

    template<typename T>
    static componentTypeId getComponentType();

    template<typename T>
    static eventTypeId getEventType();

    virtual std::list<chunkId> getListOfLoadedChunks() = 0;

    virtual std::pair<Component *, instanceId> deserializeComponent(json j) = 0;

    virtual void saveWorld() = 0;

};


template<typename T>
componentTypeId WorldSystemInterface::getComponentType() {
    componentTypeId key = stringHash(typeid(T).name());
    return key;
}

template<typename T>
componentTypeId WorldSystemInterface::getEventType() {
    eventTypeId key = stringHash(typeid(T).name());
    return key;
}

#endif //PRIZRAK_WORLDSYSTEMINTERFACE_H
