//
// Created by justin on 12/27/21.
//

#ifndef PRIZRAK_APPEARANCECOMPONENT_H
#define PRIZRAK_APPEARANCECOMPONENT_H


#include "Component.h"

class AppearanceComponent : public Component {
public:
    Appearance a;
    json serializationFunction(void) override;
    static Component *deserialize(json& j, WorldSystemInterface *w, instanceId iid);

};


#endif //PRIZRAK_APPEARANCECOMPONENT_H
