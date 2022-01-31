//
// Created by Justin.Suess on 12/26/2021.
//

#include "InputHandlingComponent.h"

json InputHandlingComponent::serializationFunction(void) {
    return nlohmann::json();
}
//should never be called
Component *InputHandlingComponent::deserialize(json& j, WorldSystemInterface *w, instanceId iid) {
    return nullptr;
}
