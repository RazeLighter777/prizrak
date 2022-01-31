//
// Created by Justin.Suess on 10/5/2021.
//

#ifndef PRIZRAK_ENTITY_H
#define PRIZRAK_ENTITY_H

#include <map>
#include <optional>
#include "typenames.h"
#include "WorldSystemInterface.h"
#include "Component.h"
#include "hashes.h"

struct Entity final {
private:
    friend class World;

    std::map<componentTypeId, Component *> components{};
    WorldSystemInterface *world;

    Entity(WorldSystemInterface *w) : world(w), components() {
    }

public:
    template<typename T>
    std::optional<T *> getComponentOfType();
};

template<typename T>
std::optional<T *> Entity::getComponentOfType() {
    auto hash = stringHash(typeid(T).name());
    return components.contains(hash) ? std::optional<T *>(static_cast<T *>(components[hash])) : std::nullopt;
}


#endif //PRIZRAK_ENTITY_H
