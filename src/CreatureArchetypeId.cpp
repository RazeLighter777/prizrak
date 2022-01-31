//
// Created by Justin.Suess on 10/6/2021.
//

#include "CreatureArchetypeId.h"
#include "WorldSystemInterface.h"
#include "BaseCreatureStateComponent.h"
#include "IntellegentCreatureStateComponent.h"
#include "Position.h"
#include "AppearanceComponent.h"
#include "Util.h"
json CreatureArchetypeId::serializationFunction(void) {
    return nlohmann::json({{"id", archetypeid}, {"rfid", rfid}});
}

Component *CreatureArchetypeId::deserialize(json& j, WorldSystemInterface *w, instanceId iid) {
    CreatureArchetypeId *arch = new CreatureArchetypeId();
    arch->archetypeid = j["id"];
    arch->rfid = j["rfid"];
    w->addComponentToEntity(w->deserializeComponent({
                                                            {"iid", iid},
                                                            {"tid", stringHash(typeid(AppearanceComponent).name())},
                                                            {"id",generateRandomWuid()},
                                                            {"dat",w->getRaw(arch->archetypeid)}}).first,iid);
    return arch;
}


void CreatureArchetypeId::createCreatureInWorld(WorldSystemInterface* w, instanceId  iid, std::string type_desc, position pos) {
    json j = w->getRaw(stringHash(type_desc.c_str()));
    w->addComponentToEntity(w->deserializeComponent({
                                                            {"iid", iid},
                                                            {"tid", ID(CreatureArchetypeId)},
                                                            {"id",generateRandomWuid()},
                                                            {"dat",{{"id" , stringHash(type_desc.c_str())},
                                                                            {"rfid", type_desc.c_str()}}}
                                                    }).first, iid);
    std::cout << to_string(j) << std::endl;
    w->addComponentToEntity(w->deserializeComponent({
                                                                            {"iid", iid},
                                                                            {"tid", stringHash(typeid(BaseCreatureStateComponent).name())},
                                                                            {"id",generateRandomWuid()},
                                                                            {"dat",{{"age" , 0},
                                                                            {"state", IDLE},
                                                                            {"stunCounter", 0},
                                                                            {"bloodLevel",100}}}
        }).first, iid);
    if (j.contains("intelligent")) {
        w->addComponentToEntity(IntellegentCreatureStateComponent::deserialize(j["intellegent"],w, iid), iid);
    }
    w->addComponentToEntity( w->deserializeComponent({

                                    {"iid", iid},
                                    {"tid", stringHash(typeid(Position).name())},
                                    {"id",generateRandomWuid()},
                                    {"dat", {
                                                    {"x",std::get<0>(pos)},
                                                    {"y",std::get<1>(pos)}
                                            }} }).first, iid);
}



