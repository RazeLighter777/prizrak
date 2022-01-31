//
// Created by Justin.Suess on 10/4/2021.
//

#include <iostream>
#include <utility>
#include <stdlib.h>
#include "EventListeningComponent.h"
#include "World.h"
#include "Util.h"
#include "WorldEventDistributionAssistor.h"

std::pair<Component *, instanceId> World::deserializeComponent(json j) {
    std::string s = j.dump();
    componentTypeId tid;
    componentInstanceId id;
    instanceId iid;
    Component *c = nullptr;
    //std::string test = j["dat"]["id"];
    //Read first 8 bytes of serialization
    tid = j["tid"];
    if (!j.contains("id")) {
        id = generateRandomWuid();
        j["id"] = id;
    } else {
        id = j["id"];
    }
    if (j.contains("iid")) {
        iid = j["iid"];
    } else {
        iid = generateRandomWuid();
    }
    auto it = componentConstructors.begin();
    /*for (it; it != componentConstructors.end(); it++) {
        if (it->first == tid) {
            c = (it->second)(j["dat"], this, iid);
            break;
        }
    }*/
    if (componentConstructors.contains(tid))
        c = (componentConstructors[tid])(j["dat"], this, iid);
    if (c == nullptr) {
        std::clog << "Warning: Could not find type " << tid << std::endl;
    }
    c->componentId = id;
    c->typeId = tid;
    c->parent = iid;
    //components[id] = c;
    /*if (componentsByType.count(tid) == 0) {
        auto v = std::list<componentInstanceId>();
        v.push_back(id);
        componentsByType.insert({tid, v});
    } else {
        componentsByType[tid].push_back(id);
    }
    if (entityWithIdExists(iid)) {
        entities[iid]->components[tid] = c;
    } else {
        entities[iid] = new Entity(this);
        entities[iid]->components[tid] = c;
    }*/
    return std::pair<Component *, instanceId>(c, iid);
}

//O(n)
bool World::entityWithIdExists(instanceId id) const {
    return entities.contains(id);
}

Entity &World::getEntityById(instanceId iid) {
    if (!entityWithIdExists(iid)) {
        throw std::invalid_argument("Entity does not exist");
    }
    return *entities[iid];
}

bool World::componentTypeIdMatches(componentTypeId ctid, componentInstanceId ciid) {
    if (components.contains(ciid)) {
        if (components[ciid]->typeId == ctid) {
            return true;
        }
    }
    return false;
}

void World::registerSystem(System *s) {
    systems.push_back(s);
}

void World::runTick() {
//delete used events.
    auto systemPairs = std::vector<std::pair<System *, int>>();
    auto matches = std::vector<std::pair<std::vector<componentTypeId>, std::pair<std::vector<System *>, std::vector<instanceId>>>>();
    //organize systems into buckets
    for (System *s : systems) {
        std::vector<componentTypeId> systemKey = s->checkoutMatchingEntities();
        if (systemKey.empty()) {
            continue;
        }
        bool insertNew = true;
        int i = 0;
        for (auto s2 : matches) {
            if (s2.first == systemKey) {
                s2.second.first.push_back(s);
                systemPairs.emplace_back(s, i);
                insertNew = false;
                break;
            }
            i++;
        }
        if (insertNew) {
            matches.emplace_back(systemKey,
                                 std::pair<std::vector<System *>, std::vector<instanceId>>(std::vector<System *>({s}),
                                                                                         std::vector<instanceId>()));
            systemPairs.emplace_back(s, matches.size() - 1);
        }
    }
    //Find the matching entities
    for (auto &m : matches) {
        //Add entities that match the primary key
        auto primaryKey = m.first[0];
        for (componentInstanceId componentId : componentsByType[primaryKey]) {
            instanceId e =  getGenericComponentByID(componentId).value()->parent;
            m.second.second.push_back(e);
        }
        //Remove matches that don't have the remainder of the keys.
        for (uint32_t i = 1; i < m.first.size(); i++) {
            for (instanceId e : m.second.second) {
                if (!getEntityById(e).components.contains(m.first[i])) {
                    m.second.second.erase(m.second.second.begin() + i);
                }
            }
        }
    }
//distribute the events so they can be handled by the systems.

    //execute each event handler.
    for (auto &handlepair : eventsByType) {
        std::list<Event *> eventsHandled;
        for (eventInstanceId eid: handlepair.second) {
            eventsHandled.push_back(events[eid]);
        }
        eventHandlers[handlepair.first]->handleEntitiesWithEvent(eventsHandled);
    }
    std::list<instanceId> listeners;
    std::list<Event*> es;
    for (auto c : componentsByType[ID(EventListeningComponent)]) {
        listeners.push_back(components[c]->parent);
    }
    for (auto e : events) {
        es.push_back(e.second);
    }
    WorldEventDistributionAssistor::distributeEvents(es,listeners,this);
    eventsByType.clear();

    for (auto& p : events) {
        oldEvents.push_back(p.second);
    }
    events.clear();
    //execute each system.
    for (auto &syspair : systemPairs) {
        syspair.first->process(this, matches[syspair.second].second.second);
    }

    //Delete marked entities/components
    for (instanceId iid : entitiesMarkedForDeletion) {
        deleteEntity(iid);
    }
    entitiesMarkedForDeletion.clear();
    for (componentInstanceId cid : componentsMarkedForDeletion) {
        deleteComponent(cid);
    }
    componentsMarkedForDeletion.clear();
}

std::optional<Component *> World::getGenericComponentByID(componentInstanceId uuid) {
    if (components.contains(uuid)) {
        return std::optional<Component *>(components[uuid]);
    }
    return std::nullopt;
}


void World::markEntityForDeletion(instanceId id) {
    entitiesMarkedForDeletion.push_back(id);
}

void World::markComponentForDeletion(componentInstanceId id) {
    componentsMarkedForDeletion.push_back(id);
}

bool World::deleteEntity(instanceId id) {
    //Delete all the component instance id fom entitiy.
    if (!entityWithIdExists(id)) {
        std::clog << "Tried to delete entity that doesn't exist. " << std::endl;
        return false;
    }
    for (auto &m : getEntityById(id).components) {
        auto &vec = componentsByType[m.second->typeId];
        vec.erase(std::remove(vec.begin(), vec.end(), m.second->componentId), vec.end());
        components.erase(m.second->componentId);
        delete m.second;
    }
    auto pos = positions[id];
    positions.erase(id);
    auto &vec = chunkMap[getChunkAtPosition(pos)][std::get<0>(pos)][std::get<1>(pos)];
    vec.erase(std::remove(vec.begin(), vec.end(), id), vec.end());
    delete entities[id];
    entities.erase(id);
    return true;
}

bool World::deleteComponent(componentInstanceId cid) {
    if (!getGenericComponentByID(cid).has_value()) {
        return false;
    }
    Component *c = getGenericComponentByID(cid).value();
    auto &vec = componentsByType[c->typeId];
    int idcopy = c->componentId;
    delete c;
    vec.erase(std::remove(vec.begin(), vec.end(), idcopy), vec.end());
    components.erase(idcopy);
    return true;
}

int World::getNumComponents() const {
    return components.size();
}

void World::registerEventHandlingSystem(System *s) {
    systems.push_front(s);
}


std::vector<instanceId> World::getEntitiesAtPosition(position pos) {
    chunkId chunk = getChunkAtPosition(pos);
    if (!chunkMap.contains(chunk)) {
        return std::vector<instanceId>();
    }
    //delete entities that aren't in the chunk
    std::vector<instanceId> res = std::vector<instanceId>();
    int inum = 0;
    auto enList = chunkMap[chunk][std::get<0>(pos) % CHUNK_X_SIZE][std::get<1>(pos) % CHUNK_Y_SIZE];
    for (instanceId i : enList) {
        /*if (entityWithIdExists(i)) {
            res.push_back(i);
        } else {

        }*/
        res.push_back(i);
        inum++;
    }
    return res;

}

chunkId World::getChunkAtPosition(position pos) {
    return (chunkId) (((chunkId) (std::get<0>(pos) / CHUNK_X_SIZE) << 32) |
                      (chunkId) (std::get<1>(pos) / CHUNK_Y_SIZE));
}

instanceId World::createEntityFromComponents(std::vector<json> j) {
    size_t n = j.size();
    instanceId id = generateRandomWuid();
    auto *e = new Entity(this);
    for (uint32_t i = 0; i < n; i++) {
        auto build = deserializeComponent(j[i]);
        build.first->parent = id;
        addComponentToEntity(build.first, id);
    }
    //positions[id] = &pos;
    return id;
}

void World::addComponentToEntity(Component *build, instanceId id) {
    components[build->componentId] = build;
    componentInstanceId cid = build->componentId;
    if (cid == 0) {
        cid = generateRandomWuid();
    }
    if (componentsByType.count(build->typeId) == 0) {
        auto v = std::list<componentInstanceId>();
        v.push_back(cid);
        componentsByType.insert({build->typeId, v});
    } else {
        componentsByType[build->typeId].emplace_back(cid);
    }
    if (entityWithIdExists(id)) {
        entities[id]->components[build->typeId] = build;
    } else {
        entities[id] = new Entity(this);
        entities[id]->components[build->typeId] = build;
    }
}

void World::registerEventHandlingSystem(eventTypeId id, EventHandler *eventHandler) {
    eventHandlers[id] = eventHandler;
}

void World::queueEvent(Event *event) {
    events[event->eid] = event;
    if (eventsByType.contains(event->typeId)) {
        eventsByType[event->typeId].push_back(event->eid);
    } else {
        eventsByType[event->typeId] = std::list<eventInstanceId>({event->eid});
    }
}

void World::loadWorldFromDatabase(const char *filename) {
    int rc = sqlite3_open(filename, &db);
    if (rc != SQLITE_OK) {
        std::cerr << "Could not open database of " << filename
                  << " or some other sqlite bullshit error happened idk Im just a computer program.\n";
        sqlite3_close(db);
    }
    //Run the schema.
    createNewTablesIfNotExists();
    //Select the components of entities that were already loaded.
    //char *zErrMsg[100];
    sqlite3_exec(db, "PRAGMA synchronous = OFF", NULL, NULL, NULL);
    sqlite3_exec(db, "PRAGMA journal_mode = MEMORY", NULL, NULL, NULL);
    sqlite3_exec(db,
                 "SELECT components.componentId, components.typeId, components.dat, entities.entityId FROM components JOIN entities ON entities.entityId = components.entityId JOIN chunks ON chunks.chunkId = entities.chunkId WHERE chunks.loaded = 1;",
                 deserializeComponentsCallback, this, NULL);
    sqlite3_exec(db, "SELECT chunks.chunkId, chunks.chunkDat FROM chunks WHERE chunks.loaded = 1;",
                 deserializeChunkBlocksCallback, this, NULL);
    //std::clog << zErrMsg << std::endl;

}

void World::createNewTablesIfNotExists() {
    const char *sql = "CREATE TABLE IF NOT EXISTS chunks (chunkId BLOB PRIMARY KEY, chunkDat BLOB, loaded INT);"
                      "CREATE TABLE IF NOT EXISTS entities (entityId BLOB PRIMARY KEY, chunkId BLOB, FOREIGN KEY(chunkId) REFERENCES chunks(chunkId));"
                      "CREATE TABLE IF NOT EXISTS components (componentId BLOB PRIMARY KEY, typeId INT, dat TEXT, entityId BLOB, FOREIGN KEY(entityId) REFERENCES entities(entityId));";
    sqlite3_exec(db, sql, 0, 0, NULL);
    /*componentId
            componentTypeId
    entityId
            dat*/
}

World::~World() {
    sqlite3_close(db);
}

int World::deserializeComponentsCallback(void *world, int argc, char **argv, char **azColName) {
    componentInstanceId cid;
    componentTypeId tid;
    std::string dat;
    instanceId iid;
    std::memcpy(&cid, argv[0], sizeof(componentInstanceId));
    tid = atoi(argv[1]);
    std::memcpy(&iid, argv[3], sizeof(instanceId));

    auto p = ((World *) world)->deserializeComponent(json({
                                                                  {"id",  cid},
                                                                  {"tid", tid},
                                                                  {"dat", json::parse(argv[2])},
                                                                  {"iid", iid}
                                                          }));
    std::cout << "Deserialized component";
    ((World *) world)->addComponentToEntity(p.first, p.second);
    return 0;
}

void World::entityPositionUpdate(instanceId id, position p, bool loadChunks) {
    chunkId c = getChunkAtPosition(p);
    if (!positions.contains(id)) {
        //positionMutex.lock();
        positions[id] = p;
        position relp = convertToChunkRelativePosition(p);
        chunkMap[getChunkAtPosition(p)][std::get<0>(relp)][std::get<1>(relp)].emplace_back(id);
        entitiesByChunk[getChunkAtPosition(p)].push_back(id);
        if (!blocks.contains(c) && loadChunks) {
            if (chunkNotInDb(c)) {
                generateChunk(c);
            } else {
                loadChunk(c);
            }
        }
        //positionMutex.unlock();
        return;
    }
    position oldpos = positions[id];
    position rel = convertToChunkRelativePosition(p);
    auto &vec = chunkMap[getChunkAtPosition(oldpos)][std::get<0>(convertToChunkRelativePosition(oldpos))][std::get<1>(convertToChunkRelativePosition(oldpos))];

    auto &l = entitiesByChunk[getChunkAtPosition(oldpos)];
    //positionMutex.lock();

    //TODO: LOAD THE CHUNK OF THE NEW POS IF IT ISN'T LOADED.

    //generateOrLoad(c);
    l.erase(std::remove(l.begin(), l.end(), id), l.end());
    entitiesByChunk[getChunkAtPosition(p)].push_back(id);
    positions[id] = p;
    vec.erase(std::remove(vec.begin(), vec.end(), id), vec.end());
    chunkMap[getChunkAtPosition(p)][std::get<0>(rel)][std::get<1>(rel)].push_back(id);
    //positionMutex.unlock();
}

void World::generateOrLoad(chunkId c) {
    if (!blocks.contains(c)  || blocks[c][0][0] == 0) {
        if (chunkNotInDb(c)) {
            generateChunk(c);
        } else {
            //called on bad encounter
            loadChunk(c);
        }
    }
}

position World::convertToChunkRelativePosition(position p) {
    return position({std::get<0>(p) & (CHUNK_X_SIZE - 1), std::get<1>(p) & (CHUNK_Y_SIZE - 1)});
}

void World::entityPositionRemoval(instanceId id) {
    position &pos = positions[id];
    auto chunkrelpos = convertToChunkRelativePosition(pos);
    auto &vec = chunkMap[getChunkAtPosition(pos)][std::get<0>(chunkrelpos)][std::get<1>(chunkrelpos)];
    auto &l = entitiesByChunk[getChunkAtPosition(pos)];
    //positionMutex.lock();
    positions.erase(id);
    vec.erase(std::remove(vec.begin(), vec.end(), id), vec.end());
    l.erase(std::remove(l.begin(), l.end(), id), l.end());
    //positionMutex.unlock();
}

void World::loadEntityExplicity(instanceId iid) {
    if (entityWithIdExists(iid)) {
        return;
    }
    char hexString[20];
    sprintf(hexString, "%016llx", iid);
    auto s = std::string(
            "SELECT components.componentId, components.typeId, components.dat, components.entityId FROM components WHERE components.entityId = x'") +
             hexString + "'";
    sqlite3_exec(db, s.c_str(), deserializeComponentsCallback, this, NULL);
}

void World::saveWorld() {
    //Set all chunks in the database to unloaded.
    //Insert all of the chunks
    sqlite3_exec(db, "BEGIN TRANSACTION;", NULL, NULL, NULL);
    sqlite3_exec(db, "UPDATE chunks SET loaded = 0;", NULL, NULL, NULL);
    for (auto &c : blocks) {
        if (c.second[0][0] == 0) {
            std::clog << "Chunk skipped " << std::endl;
            continue;
        }
        std::clog << "Saving chunk" << std::endl;
        chunkId val = c.first;
        const char *sql = "INSERT OR REPLACE INTO chunks VALUES (?, ?, ?)";
        blockTypeId chunkCopy[CHUNK_X_SIZE * CHUNK_Y_SIZE];
        int i = 0;
        auto &chunk = blocks[val];
        for (int x = 0; x < CHUNK_X_SIZE; x++) {
            for (int y = 0; y < CHUNK_Y_SIZE; y++) {
                //std::cout << "block value : " << chunk[x][y];
                chunkCopy[i] = chunk[x][y];
                i++;
            }
        }
        int loaded = true;
        sqlite3_prepare_v2(db, sql, -1, &res, NULL);
        sqlite3_bind_blob(res, 1, &val, sizeof(val), SQLITE_STATIC);
        sqlite3_bind_blob(res, 2, chunkCopy, CHUNK_X_SIZE * CHUNK_Y_SIZE * sizeof(blockTypeId), SQLITE_STATIC);
        sqlite3_bind_int(res, 3, loaded);
        sqlite3_step(res);
    }
    for (auto &e : entities) {
        position pos;
        chunkId val;
        //Different insert depending on if it has a position.
        const char *sql = "INSERT OR REPLACE INTO entities (entityId, chunkId) VALUES (?, ?)";
        if (sqlite3_prepare_v2(db, sql, -1, &res, NULL)) {
            std::cerr << "Error inserting deez nuts into the sqlite database.\n";
        }
        sqlite3_bind_blob(res, 1, &e.first, sizeof(e.first), SQLITE_STATIC);
        //sqlite3_bind_int64(res, 1, e.first);
        auto positionComponent = e.second->getComponentOfType<Position>();
        if (positionComponent.has_value()) {
            pos = positionComponent.value()->get();
            val = getChunkAtPosition(pos);
            sqlite3_bind_blob(res, 2, &val, sizeof(val), SQLITE_STATIC);
            //sqlite3_bind_int64(res, 2, val);
        } else {
            sqlite3_bind_null(res, 2);
        }
        sqlite3_step(res);
    }
    //Insert all of the components.
    for (auto &c : components) {
        const char *sql = "INSERT OR REPLACE INTO components VALUES (?, ?, ?, ?)";
        auto j0 = c.second->serializationFunction();
        if (j0 == nullptr) {
            continue;
        }
        std::string s0 = j0.dump();
        const char *s1 = s0.c_str();
        instanceId iid = c.second->parent;
        sqlite3_prepare_v2(db, sql, -1, &res, NULL);
        sqlite3_bind_blob(res, 1, &c.first, sizeof(c.first), SQLITE_STATIC);
        sqlite3_bind_int(res, 2, c.second->typeId);
        //TODO: Change to plaintext for debug builds.
        sqlite3_bind_text(res, 3, s1, strlen(s1), SQLITE_STATIC);
        sqlite3_bind_blob(res, 4, &iid, sizeof(iid), SQLITE_STATIC);
        sqlite3_step(res);
    }
    sqlite3_exec(db, "END TRANSACTION;", NULL, NULL, NULL);
}



void World::loadChunk(chunkId id) {
    SwapBinary(id);
    loadChunkBlocks(id);
    char hexString[20];
    std::clog << "World loadChunk called" << std::endl;
    sprintf(hexString, "%016llx", id);

    auto sql = std::string("SELECT components.componentId, components.typeId, components.dat, entities.entityId FROM components JOIN entities ON entities.entityId = components.entityId WHERE entities.chunkId = x'") +
                                                                                                                                                                                                                   hexString + "'";
    sqlite3_exec(db, sql.c_str(), deserializeComponentsCallback, this, NULL);

}

blockTypeId World::getBlockTypeAtPosition(position pos) {
    //position rel = convertToChunkRelativePosition(pos);
    /*if (!blocks.contains(getChunkAtPosition(pos))) {
        std::cerr << "Chunk does not exist" << std::endl;

    }*/
    //optimized version
    return blocks[getChunkAtPosition(pos)][std::get<0>(pos) & (CHUNK_X_SIZE - 1)][std::get<1>(pos) & (CHUNK_Y_SIZE - 1)];
}

void World::registerBlockType(BlockType *t) {
    std::clog << "Registered block type with id " << t->getType() << std::endl;
    blockTypes.insert({t->getType(), t});
}

BlockType *World::getBlockProperties(blockTypeId id) {
    if (id == 0) {
        return &air;
    }
    //COMMENTED OUT FOR PERFORMANCE REASONS. MAY INTRODUCE UNINFORMATIVE CRASHES WITH INCOMPATIBLE SAVE FILES
    /*if (!blockTypes.contains(id)) {
        std::cerr << "Could not find block with type " << id << std::endl;
    }*/
    return this->blockTypes[id];
}
int World::deserializeChunkBlocksCallback(void *world, int argc, char **argv, char **azColName) {
    int i = 0;
    chunkId id;
    std::memcpy(&id, argv[0], sizeof(chunkId));
    std::cout << "Chunk loaded with id " << id << " at position " << std::get<0>(World::getPositionOfChunk(id)) << std::get<0>(World::getPositionOfChunk(id)) <<  std::endl;
    for (int x = 0; x < CHUNK_X_SIZE; x++) {
        for (int y = 0; y < CHUNK_Y_SIZE; y++) {
            auto d2 = argv[1][i];
            auto d1 = argv[1][i + 1];
            if (d2 == 0 && d1 == 0) {
                std::cout << "SHITS BUSSTED" << std::endl;
            }
            blockTypeId bid;
            std::memcpy(&bid, &d2, sizeof(d2));
            std::memcpy((char *) &bid + 1, &d1, sizeof(d1));
            ((World *) world)->blocks[id][x][y] = bid;
            i += 2;
        }
    }
    return 0;
}

void World::loadChunkBlocks(chunkId id) {
    char hexString[20];
    sprintf(hexString, "%016llx", id);
    auto s = std::string("SELECT chunks.chunkId, chunks.chunkDat FROM chunks WHERE chunks.chunkId = x'") + hexString +
             "';";
    sqlite3_exec(db, s.c_str(), deserializeChunkBlocksCallback, this, NULL);
}

void World::saveChunkBlocks(chunkId id) {
    std::clog << "Saving chunk " << std::endl;
    char hexString[20];
    sprintf(hexString, "%016llx", id);
    auto prefix = std::string("INSERT OR REPLACE INTO chunks VALUES (x'") + hexString + "', ?, ?)";
    std::clog << "PREFIX: " << prefix << std::endl;
    blockTypeId chunkCopy[CHUNK_X_SIZE * CHUNK_Y_SIZE];
    int i = 0;
    generateChunk(id);
    for (int x = 0; x < CHUNK_X_SIZE; x++) {
        for (int y = 0; y < CHUNK_Y_SIZE; y++) {
            //std::cout << "block value : " << chunk[x][y];
            chunkCopy[i] = blocks[id][x][y];
            i++;
        }
    }
    int loaded = false;
    sqlite3_prepare_v2(db, prefix.c_str(), -1, &res, NULL);
    sqlite3_bind_blob(res, 1, chunkCopy, CHUNK_X_SIZE * CHUNK_Y_SIZE * sizeof(blockTypeId), SQLITE_STATIC);
    sqlite3_bind_int(res, 2, loaded);
    sqlite3_step(res);
}

void World::setWorldGenerator(WorldGenerator *wg) {
    this->wg = wg;
}

//gets full path block/stone
blockTypeId World::getBlockTypeFromRaws(std::string s) {
    return stringHash((s).c_str());
}

void World::createBlockAtPositionById(blockTypeId id, position pos) {
    position rel = convertToChunkRelativePosition(pos);
    blocks[getChunkAtPosition(pos)][std::get<0>(rel)][std::get<1>(rel)] = id;
}

void World::deleteBlockAtPosition(position pos) {
    position rel = convertToChunkRelativePosition(pos);
    blocks[getChunkAtPosition(pos)][std::get<0>(rel)][std::get<1>(rel)] = 0;
}

void World::generateChunkIfNotExists(chunkId id) {

}

void World::loadRaws(std::list<std::pair<uint16_t , json>> &r) {
    for (auto &raw : r) {
        raws.insert(raw);
    }
}

void World::generateChunk(chunkId id) {
    std::clog << "Generating new chunk . . . " << std::endl;
    blocks.insert({id, std::array<std::array<blockTypeId, CHUNK_Y_SIZE>, CHUNK_X_SIZE>()});
    wg->generateChunk(id, &blocks[id], this);
}


bool World::chunkLoaded(chunkId id) const {
    return blocks.contains(id);
}

position World::getPositionOfChunk(chunkId id) {
    return {((id >> 32) & 0xFFFFFFFF) * CHUNK_X_SIZE, (id & 0xFFFFFFFF) * CHUNK_Y_SIZE};
}

bool World::chunkNotInDb(chunkId c) {
    SwapBinary(c);
    char hexString[20];
    std::clog << "ChunkNotInDb called" << std::endl;
    sprintf(hexString, "%016llx", c);
    auto s = std::string("SELECT * FROM chunks WHERE chunks.chunkId = x'") + hexString + "'";
    std::clog << s << std::endl;
    int rc = sqlite3_prepare_v2(db, s.c_str(), -1, &res, NULL);
    int step = sqlite3_step(res);
    if (step != SQLITE_ROW) {
        if (step == SQLITE_DONE) {
            std::cout << "Chunk wasn't in database" << std::endl;
            return true;
        }
    }
    std::cout << "Chunk was in database" << std::endl;
    return false;

}

const json& World::getRaw(uint16_t name) {
    return raws[name];
}

void World::unloadChunk(chunkId id) {
    saveWorld();
    for (auto& e : entitiesByChunk[id]) {
/*
        position pos;
        chunkId val;
        //Different insert depending on if it has a position.
        const char *sql = "INSERT OR REPLACE INTO entities (entityId, chunkId) VALUES (?, ?)";
        if (sqlite3_prepare_v2(db, sql, -1, &res, NULL)) {
            std::cerr << "Error inserting deez nuts into the sqlite database.\n";
        }
        sqlite3_bind_blob(res, 1, &e, sizeof(e), SQLITE_STATIC);
        //sqlite3_bind_int64(res, 1, e.first);
        auto positionComponent = getEntityById(e).getComponentOfType<Position>();
        if (positionComponent.has_value()) {
            pos = positionComponent.value()->get();
            val = getChunkAtPosition(pos);
            sqlite3_bind_blob(res, 2, &val, sizeof(val), SQLITE_STATIC);
            //sqlite3_bind_int64(res, 2, val);
        } else {
            sqlite3_bind_null(res, 2);
        }
        sqlite3_step(res);
*/
        markEntityForDeletion(e);
    }
    blocks[id][0][0] = 0;
    blocks.erase(id);
}

std::list<chunkId> World::getListOfLoadedChunks() {
    std::list<chunkId> res;
    for (auto c : blocks) {
        res.push_back(c.first);
    }
    return res;
}

unsigned int World::getNumberOfLoadedEntities() const {
    return entities.size();
}

void World::deleteOldEvents() {
    for (Event* e : oldEvents) {
        delete e;
    }
    oldEvents.clear();
}

World::World()  {
    blocks.reserve(100);
    entities.reserve(100);
    entitiesByChunk.reserve(100);
}

Biome* World::getBiomeAtPosition(position pos) {
    return wg->getBiome(pos);
}

