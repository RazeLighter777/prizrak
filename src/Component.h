//
// Created by Justin.Suess on 10/4/2021.
//

#ifndef PRIZRAK_COMPONENT_H
#define PRIZRAK_COMPONENT_H

#include "typenames.h"
#include "json.hpp"
#include "WorldSystemInterface.h"

using nlohmann::json;

struct Component {
    componentTypeId typeId = 0;
    componentInstanceId componentId = generateRandomWuid();
    instanceId parent = 0;

    virtual json serializationFunction(void) = 0;

    virtual ~Component() {};
};

typedef Component *(*componentDeserializationFunction)(json& injson, WorldSystemInterface *world,
                                                       instanceId iid); // function pointer type

#endif //PRIZRAK_COMPONENT_H
