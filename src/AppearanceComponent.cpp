//
// Created by justin on 12/27/21.
//

#include "AppearanceComponent.h"
#include <iostream>
Component *AppearanceComponent::deserialize(json& j, WorldSystemInterface *w, instanceId iid) {
    //std::clog << "APPEARANCE JSON " << to_string(j) << std::endl;
    auto r = new AppearanceComponent();
    r->a.animation = STATIC;
    r->a.layer = GROUND;
    if (j.contains("code")) {
        const spriteId i = j["code"].get<int>();
        //TODO: Add background colors
        r->a.spriteIds.push_back({i, TCOD_color_RGB(j["fg"]["r"], j["fg"]["g"], j["fg"]["b"]), {255, 0, 255}});
    } else {
        r->a.spriteIds.push_back({47, {255, 0, 255}, {255, 0, 255}});
    }
    return r;
}

json AppearanceComponent::serializationFunction(void) {
    return nullptr;
}
