//
// Created by Justin.Suess on 10/30/2021.
//

#ifndef PRIZRAK_INTELLEGENTCREATURESTATS_H
#define PRIZRAK_INTELLEGENTCREATURESTATS_H

#include "typenames.h"
#include "json.hpp"
using nlohmann::json;
struct IntellegentCreatureStats {
    uint8_t intellect;
    uint8_t wisdom;
    uint8_t spirituality;
    bool canLearn;
    bool canRead;
    bool canSpeak;
    bool canCraft;
    IntellegentCreatureStats(json raw) {
        DS(intellect);
        DS(wisdom);
        DS(spirituality);
        DS(canLearn);
        DS(canRead);
        DS(canSpeak);
        DS(canCraft);
    }
};


#endif //PRIZRAK_INTELLEGENTCREATURESTATS_H
