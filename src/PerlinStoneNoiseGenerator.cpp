//
// Created by Justin.Suess on 10/26/2021.
//

#include <iostream>
#include <limits>
#include <execution>
#include "PerlinStoneNoiseGenerator.h"
#include "World.h"
#include "libtcod.hpp"
#include "typenames.h"
#include "Util.h"
#include "CreatureArchetypeId.h"
void PerlinStoneNoiseGenerator::generateChunk(chunkId id,
                                              std::array<std::array<blockTypeId, CHUNK_Y_SIZE>, CHUNK_X_SIZE> *blocks,
                                              WorldSystemInterface *worldSystemInterface) {
    auto bid = worldSystemInterface->getBlockTypeFromRaws("block/stone");
    auto bid2 = worldSystemInterface->getBlockTypeFromRaws("block/stonefloor");
    auto pos = World::getPositionOfChunk(id);
    //std::cout << std::get<0>(pos) << " : " << std::get<1>(pos) << std::endl;
    int index = 0;
    std::vector<int> range;
    for (int i = 0; i < CHUNK_X_SIZE; i++) {
        range.emplace_back(i);
    }
    std::for_each(std::execution::par_unseq,range.begin(), range.end(), [this, &blocks, pos] (auto x) {
        for (int y = 0; y < CHUNK_Y_SIZE; y++) {
            Biome* b = getBiome(position({std::get<0>(pos) + x, std::get<1>(pos) + y}));
            float p[2] = {(float) (std::get<0>(pos) + x) / float(UINT16_MAX) * 1000,
                          (float) (std::get<1>(pos) + y) / float(UINT16_MAX) * 1000};
            //std::cout << p[0] << " / " << p[1] << std::endl;
            //std::cout << (noise->get(p, TCOD_NOISE_SIMPLEX) + 1.) << std::endl;
            (*blocks)[x][y] = (noise->get(p, TCOD_NOISE_SIMPLEX) + b->getRockiness()) > .99 ? b->getStructuredBlock({x,y}) : b->getBaseBlock({x,y});

        }
    });
}

float PerlinStoneNoiseGenerator::getTemperature(position coords) {
    return getRandomFloat(coords) / RAND_MAX  * 100;
}

uint16_t PerlinStoneNoiseGenerator::getAltitude(position coords) {
    return getRandomFloat(coords) / RAND_MAX  * 100;
}

float PerlinStoneNoiseGenerator::getHumidity(position coords) {
    return getRandomFloat(coords) / RAND_MAX  * 100;
}

float PerlinStoneNoiseGenerator::getRandomFloat(position coords) {
    srand(GlobalGameState::getInstance().getSeed() % (1+(std::get<0>(coords) ^ (33 * std::get<1>(coords)))));
    return rand();
}

Biome *PerlinStoneNoiseGenerator::getBiome(std::tuple<uint16_t, uint16_t> tuple) {
    float p[2] = {static_cast<float>(std::get<0>(tuple) / 128.), static_cast<float>(std::get<1>(tuple) / 128.)};
    float temp = pow(((noise->get(p)/2.)+.9),2)*59;
    //std::cout << temp << std::endl;
    uint16_t minBiome = 0;
    float minDistance = std::numeric_limits<float>::max();
    uint16_t i = 0;
    for (Biome* b : loadedBiomes) {
        float distance = abs(b->idealTemperature() - temp);
        if (distance < minDistance) {
            minBiome = i;
            minDistance = distance;
        }
        i++;
    }
    return loadedBiomes[minBiome];
}
