//
// Created by justin on 1/31/22.
//

#ifndef PRIZRAK_ITEMARCHETYPEID_H
#define PRIZRAK_ITEMARCHETYPEID_H

#include "Component.h"

struct ItemArchetypeId : public Component {

public:

    static void createItemInWorld(WorldSystemInterface* w, instanceId  iid, std::string type_desc, position pos);

    static void createItemInInventory(WorldSystemInterface* w, instanceId  iid, std::string type_desc, instanceId inv);

    uint16_t archetypeid;

    std::string rfid;

    json serializationFunction(void) override;

    static Component *deserialize(json& j, WorldSystemInterface *w, instanceId iid);

};

#endif //PRIZRAK_ITEMARCHETYPEID_H
