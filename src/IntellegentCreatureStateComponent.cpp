//
// Created by Justin.Suess on 10/30/2021.
//

#include "IntellegentCreatureStateComponent.h"
#include "typenames.h"
json IntellegentCreatureStateComponent::serializationFunction(void) {
    return {{"state",state}};
}

Component *IntellegentCreatureStateComponent::deserialize(json& j, WorldSystemInterface *w, instanceId iid) {
    IntellegentCreatureStateComponent* state = new IntellegentCreatureStateComponent();
    state->state = j["state"];
    return state;
}
