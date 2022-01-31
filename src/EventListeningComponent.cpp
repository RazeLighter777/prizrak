//
// Created by justin on 1/23/22.
//

#include "EventListeningComponent.h"

json EventListeningComponent::serializationFunction(void) {
    return nullptr;
}

Component *EventListeningComponent::deserialize(json& j, WorldSystemInterface *w, instanceId iid) {
    return new EventListeningComponent();
}
