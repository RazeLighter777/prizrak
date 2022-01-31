//
// Created by justin on 1/31/22.
//

#include "ItemArchetypeId.h"
#include <iostream>
json ItemArchetypeId::serializationFunction(void) {
    return nlohmann::json({{"id", archetypeid}, {"rfid", rfid}});
}

Component *ItemArchetypeId::deserialize(json& j, WorldSystemInterface *w, instanceId iid) {
    ItemArchetypeId *arch = new ItemArchetypeId();
    return arch;
}


void ItemArchetypeId::createItemInWorld(WorldSystemInterface* w, instanceId  iid, std::string type_desc, position pos) {
    json j = w->getRaw(stringHash(type_desc.c_str()));
    w->addComponentToEntity(w->deserializeComponent({
                                                            {"iid", iid},
                                                            {"tid", ID(ItemArchetypeId)},
                                                            {"id",generateRandomWuid()},
                                                            {"dat",{{"id" , stringHash(type_desc.c_str())},
                                                                            {"rfid", type_desc.c_str()}}}
                                                    }).first, iid);
}

void
ItemArchetypeId::createItemInInventory(WorldSystemInterface *w, instanceId iid, std::string type_desc, instanceId inv) {
    //TODO : add item placement in an inventory.
    json j = w->getRaw(stringHash(type_desc.c_str()));
    w->addComponentToEntity(w->deserializeComponent({
                                                            {"iid", iid},
                                                            {"tid", ID(ItemArchetypeId)},
                                                            {"id",generateRandomWuid()},
                                                            {"dat",{{"id" , stringHash(type_desc.c_str())},
                                                                            {"rfid", type_desc.c_str()}}}
                                                    }).first, iid);
}



