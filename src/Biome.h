//
// Created by Justin.Suess on 1/27/2022.
//

#ifndef PRIZRAK_BIOME_H
#define PRIZRAK_BIOME_H

#include <vector>
#include "typenames.h"
#include "json.hpp"
#include "WorldGenerator.h"

class Biome {
private:
    std::vector<blockTypeId> base,structured;
    float weight = 1.;
    float temp;
    uint16_t altitude;
    float humidity;
    float rockiness;
    WorldSystemInterface* w;
    WorldGenerator* wg;
public:

    Biome(nlohmann::json& constructor, WorldGenerator* worldGenerator, WorldSystemInterface* world);

    blockTypeId getBaseBlock(position p );

    blockTypeId getStructuredBlock(position p);

    void generateBiomeStructure(position absolutePosition);

    float biomeWeight();

    float getRockiness();

    float idealTemperature();

    uint16_t idealAltitude();

    float idealHumidity();
};


#endif //PRIZRAK_BIOME_H
