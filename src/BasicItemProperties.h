//
// Created by justin on 1/31/22.
//

#ifndef PRIZRAK_BASICITEMPROPERTIES_H
#define PRIZRAK_BASICITEMPROPERTIES_H

#include <cstdint>
#include <string>
#include "json.hpp"
#include "Util.h"
enum ITEM_CATEGORY {

};

class BasicItemProperties {
public:
    uint8_t stackSize; //number of units something can be stacked
    uint32_t weight; // in kg
    uint32_t volume; // in cm^3
    std::string fullName;
    std::string description;
    BasicItemProperties(nlohmann::json& raw) {
        DS(fullName);
        DS(weight);
        DS(volume);
        DS(fullName);
        DS(stackSize);
        DS(description);
    }
};


#endif //PRIZRAK_BASICITEMPROPERTIES_H
