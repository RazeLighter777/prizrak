//
// Created by Justin.Suess on 10/16/2021.
//

#ifndef PRIZRAK_WORLDGENERATOR_H
#define PRIZRAK_WORLDGENERATOR_H

#include "typenames.h"
#include "WorldSystemInterface.h"
class Biome;

class WorldGenerator {
public:
    virtual void generateChunk(chunkId id, std::array<std::array<blockTypeId, CHUNK_Y_SIZE>, CHUNK_X_SIZE> *blocks,
                               WorldSystemInterface *worldSystemInterface) = 0;

    virtual float getTemperature(position coords) = 0;

    virtual uint16_t getAltitude(position coords) = 0;

    virtual float getHumidity(position coords) = 0;

    virtual float getRandomFloat(position p) = 0;

    virtual Biome* getBiome(position p) = 0;
};

#endif //PRIZRAK_WORLDGENERATOR_H
