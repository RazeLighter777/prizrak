//
// Created by Justin.Suess on 12/26/2021.
//

#include "PlayerComponent.h"
#include "GlobalGameState.h"
#include "EventListeningComponent.h"
#include "Util.h"
json PlayerComponent::serializationFunction(void) {
    return nlohmann::json({"player", true});
}

Component *PlayerComponent::deserialize(json& j, WorldSystemInterface *w, instanceId iid) {
    GlobalGameState::getInstance().setPlayerInstance(iid);
    w->addComponentToEntity(w->deserializeComponent({
                                                            {"iid", iid},
                                                            {"tid", ID(EventListeningComponent)},
                                                            {"id",generateRandomWuid()},
                                                            {"dat",{}}}).first,iid);
    return new PlayerComponent();
}
