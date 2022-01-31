//
// Created by Justin.Suess on 10/6/2021.
//

#ifndef PRIZRAK_CREATUREARCHETYPEID_H
#define PRIZRAK_CREATUREARCHETYPEID_H

#include <iostream>
#include "Component.h"

struct CreatureArchetypeId : public Component {

public:

    static void createCreatureInWorld(WorldSystemInterface* w, instanceId  iid, std::string type_desc, position pos);

    uint16_t archetypeid;

    std::string rfid;

    json serializationFunction(void) override;

    static Component *deserialize(json& j, WorldSystemInterface *w, instanceId iid);

};


#endif //PRIZRAK_CREATUREARCHETYPEID_H
