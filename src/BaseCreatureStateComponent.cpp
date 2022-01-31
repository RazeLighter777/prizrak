//
// Created by Justin.Suess on 10/29/2021.
//

#include "BaseCreatureStateComponent.h"
#include "typenames.h"

json BaseCreatureStateComponent::serializationFunction(void) {
    return {SF(bloodLevel), SF(state), SF(stunCounter), SF(age)};
}

Component *BaseCreatureStateComponent::deserialize(json& j, WorldSystemInterface *w, instanceId iid) {
    auto* c = new BaseCreatureStateComponent();
    c->bloodLevel = j["bloodLevel"];
    c->state = j["state"];
    c->stunCounter = j["stunCounter"];
    c->age = j["age"];
    return c;
}
