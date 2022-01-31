//
// Created by Justin.Suess on 1/27/2022.
//

#include "Biome.h"

float Biome::idealHumidity() {
    return humidity;
}

uint16_t Biome::idealAltitude() {
    return altitude;
}

float Biome::idealTemperature() {
    return temp;
}

float Biome::biomeWeight() {
    return weight;
}

void Biome::generateBiomeStructure(position absolutePosition) {
    //TODO: Allow generation of biome structures.
}

blockTypeId Biome::getStructuredBlock(position p ) {
    return structured[fmodf(wg->getRandomFloat(p),float(structured.size()))];
    //return structured[0];
}

Biome::Biome(json &raw, WorldGenerator *worldGenerator, WorldSystemInterface *world) : wg(worldGenerator), w(world) {
    DS(weight);
    DS(temp);
    DS(altitude);
    DS(humidity);
    DS(rockiness);
    for (auto& q : raw["base"]) {
        base.emplace_back(stringHash(q.get<std::string>().c_str()));
    }
    for (auto& q : raw["structured"]) {
        structured.emplace_back(stringHash(q.get<std::string>().c_str()));
    }
}

blockTypeId Biome::getBaseBlock(position p) {
    return base[fmodf(wg->getRandomFloat(p),float(base.size()))];
    //return base[0];
}

float Biome::getRockiness() {
    return rockiness;
}
