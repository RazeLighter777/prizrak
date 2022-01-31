//
// Created by Justin.Suess on 10/16/2021.
//

#include <iostream>
#include "Position.h"


Component *Position::deserialize(json& j, WorldSystemInterface *w, instanceId iid) {
    std::string s = j.dump();
    //TODO: Make serializing this shit actually work.
    position x = {j["x"], j["y"]};
    return new Position(w, x, iid);
}

void Position::operator=(position other) {
    internal = other;
    w->entityPositionUpdate(iid, internal, true);
}

void Position::operator=(const Position &other) {
    internal = other.internal;
    w->entityPositionUpdate(iid, internal, true);
}

Position::Position(WorldSystemInterface *world, position pos, instanceId id) : w(world), internal(std::move(pos)),
                                                                               iid(id) {
    //Don't load chunks when this object is created to prevent recursion. TODO: May be buggy if you spawn an entity in a non-existant chunk.
    w->entityPositionUpdate(iid, internal, false);
}

json Position::serializationFunction(void) {
    return nlohmann::json({
                                  {"x", std::get<0>(internal)},{"y",std::get<1>(internal)}
                          });
}

Position::~Position() {
    w->entityPositionRemoval(iid);
}

position Position::get() const {
    return internal;
}

