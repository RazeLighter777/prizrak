//
// Created by Justin.Suess on 10/4/2021.
//

#ifndef PRIZRAK_WORLD_H
#define PRIZRAK_WORLD_H


#include <map>
#include <vector>
#include <mutex>
#include <memory_resource>
#include <unordered_map>
#include <optional>
#include <list>
#include <sqlite3.h>
#include "Component.h"
#include "typenames.h"
#include <libtcod/libtcod.h>
#include "hashes.h"
#include "Entity.h"
#include "Wuid.h"
#include "System.h"
#include "WorldSystemInterface.h"
#include "Event.h"
#include "EventHandler.h"
#include "Position.h"
#include "BlockType.h"
#include "WorldGenerator.h"

template<class T> using AtomicSharedRef = std::atomic<std::shared_ptr<T>>;

class World final : public WorldSystemInterface {
public:
    std::pair<Component *, instanceId> deserializeComponent(json j);
    World();
    void loadChunk(chunkId id);
    unsigned int getNumberOfLoadedEntities() const;
private:
    friend class Position;
    std::mutex positionMutex;
    sqlite3 *db;
    sqlite3_stmt *res;
    WorldGenerator *wg;
    BlockType air = BlockType(json({{"id",          0},
                                    {"name",        "air"},
                                    {"solid",       false},
                                    {"gravity",     false},
                                    {"description", "literally air"},
                                    {"transparent", true},
                                    {"concealing",  false}}));
    std::list<instanceId> entitiesMarkedForDeletion = std::list<instanceId>();
    std::unordered_map<eventTypeId, std::list<eventInstanceId>> eventsByType = std::unordered_map<eventTypeId, std::list<eventInstanceId>>();
    std::unordered_map<eventInstanceId, Event *> events;
    std::map<eventTypeId, EventHandler *> eventHandlers;
    std::list<componentInstanceId> componentsMarkedForDeletion = std::list<componentInstanceId>();
    std::list<System *> systems;

    bool deleteEntity(instanceId id);
    std::unordered_map<instanceId, Entity *> entities;
    std::unordered_map<componentTypeId, componentDeserializationFunction> componentConstructors = std::unordered_map<componentTypeId, componentDeserializationFunction>();
    std::unordered_map<componentInstanceId, Component *> components = std::unordered_map<componentInstanceId, Component *>();
    std::unordered_map<componentTypeId, std::list<componentInstanceId>> componentsByType = std::unordered_map<componentTypeId, std::list<componentInstanceId>>();
    std::unordered_map<chunkId, std::array<std::array<std::list<instanceId>, CHUNK_Y_SIZE>, CHUNK_X_SIZE>> chunkMap = std::unordered_map<chunkId, std::array<std::array<std::list<instanceId>, CHUNK_Y_SIZE>, CHUNK_X_SIZE>>();
    std::unordered_map<chunkId, std::array<std::array<blockTypeId, CHUNK_Y_SIZE>, CHUNK_X_SIZE>> blocks = std::unordered_map<chunkId, std::array<std::array<blockTypeId, CHUNK_Y_SIZE>, CHUNK_X_SIZE>>();
    std::unordered_map<chunkId, std::list<instanceId>> entitiesByChunk;
    std::unordered_map<instanceId, position> positions;
    std::unordered_map<blockTypeId, BlockType *> blockTypes;
    std::list<Event*> oldEvents;

    std::unordered_map<uint16_t , json> raws;

    //void offloadChunk(chunkId id);

    void createNewTablesIfNotExists();


    static int deserializeComponentsCallback(void *world, int argc, char **argv, char **azColName);

    static int deserializeChunkBlocksCallback(void *world, int argc, char **argv, char **azColName);

    void entityPositionUpdate(instanceId id, position p, bool loadChunks) override;

    void entityPositionRemoval(instanceId id) override;

    void generateChunkIfNotExists(chunkId id);

public:

    void deleteOldEvents();

    Biome* getBiomeAtPosition(position pos) override;

    ~World();

    blockTypeId getBlockTypeFromRaws(std::string s) override;

    void createBlockAtPositionById(blockTypeId, position pos) override;

    void deleteBlockAtPosition(position pos) override;

    blockTypeId getBlockTypeAtPosition(position pos) override;

    void loadEntityExplicity(instanceId iid) override;

    const json& getRaw(uint16_t name) override;

    void loadRaws(std::list<std::pair<uint16_t , json>> &r);

    void generateChunk(chunkId id);

    bool chunkLoaded(chunkId id) const override;

    void setWorldGenerator(WorldGenerator *wg);

    static position convertToChunkRelativePosition(position p);

    void loadWorldFromDatabase(const char *filename);

    void queueEvent(Event *event) override;

    void loadChunkBlocks(chunkId id);

    void saveChunkBlocks(chunkId id);

    void registerEventHandlingSystem(eventTypeId id, EventHandler *eventHandler);

    void markEntityForDeletion(instanceId id) override;

    void markComponentForDeletion(componentInstanceId id) override;

    template<typename T>
    std::optional<T *> getComponentByID(componentInstanceId uuid) const;

    std::optional<Component *> getGenericComponentByID(componentInstanceId uuid);

    Entity &getEntityById(instanceId iid) override;

    bool entityWithIdExists(instanceId id) const override;

    template<typename T>
    componentTypeId registerComponentDeserializationFunction(componentDeserializationFunction func);

    instanceId createEntityFromComponents(std::vector<json> j);

    void addComponentToEntity(Component *component, instanceId id);

    bool componentTypeIdMatches(componentTypeId, componentInstanceId) override;

    void registerSystem(System *s);

    void registerEventHandlingSystem(System *s);

    void registerBlockType(BlockType *t);

    BlockType *getBlockProperties(blockTypeId id) override;

    void runTick();

    int getNumComponents() const;

    bool deleteComponent(componentInstanceId cid);

    static chunkId getChunkAtPosition(position pos);

    static position getPositionOfChunk(chunkId id);

    std::vector<instanceId> getEntitiesAtPosition(position pos);

    void saveWorld();

    void generateChunkIfNotExists();

    bool chunkNotInDb(chunkId c);

    void unloadChunk(chunkId id) override;

    std::list<chunkId> getListOfLoadedChunks() override;

    void generateOrLoad(chunkId c);
};


template<typename T>
std::optional<T *> World::getComponentByID(componentInstanceId uuid) const {
    static componentTypeId key = stringHash(typeid(T).name());
    if (components.at(uuid)->typeId == key) {
        return std::optional<T *>{static_cast<T *>(components.at(uuid))};
    }
    return std::nullopt;
}

template<typename T>
componentTypeId World::registerComponentDeserializationFunction(componentDeserializationFunction func) {
    static componentTypeId key = stringHash(typeid(T).name());
    componentConstructors.insert(std::make_pair(key, func));
    return key;
}


#endif //PRIZRAK_WORLD_H
