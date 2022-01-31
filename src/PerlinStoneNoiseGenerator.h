//
// Created by Justin.Suess on 10/26/2021.
//

#ifndef PRIZRAK_PERLINSTONENOISEGENERATOR_H
#define PRIZRAK_PERLINSTONENOISEGENERATOR_H


#include "WorldGenerator.h"
#include "WorldSystemInterface.h"
#include "Biome.h"
#include "libtcod.hpp"
#include "GlobalGameState.h"
class PerlinStoneNoiseGenerator final : public WorldGenerator {
    TCODNoise *noise, *tempNoise, *altitudeNoise, *humidityNoise;
    std::vector<Biome*> loadedBiomes;

public:
    float getTemperature(position coords) override;

    uint16_t getAltitude(position coords) override;

    float getHumidity(position coords) override;

    float getRandomFloat(position p) override;

private:
// 2D noise with a predefined random number generator
    TCODRandom *myRandom = new TCODRandom(GlobalGameState::getInstance().getSeed());
    TCODRandom *tempRandom = new TCODRandom(GlobalGameState::getInstance().getSeed()  + 33);
    TCODRandom *humidityRandom = new TCODRandom(GlobalGameState::getInstance().getSeed()  + 46);
    TCODRandom *altitudeRandom = new TCODRandom(GlobalGameState::getInstance().getSeed()  + 33);

public:
    PerlinStoneNoiseGenerator(WorldSystemInterface* w) {
        //TODO: Factor out passing worldsysteminterface to generatechunk
        auto j  = w->getRaw(NM("listing/biomes"));
        for (auto& q : j["biomes"]) {
            nlohmann::json z = w->getRaw(NM(("biome/" + q.get<std::string>()).c_str()));
            loadedBiomes.emplace_back(new Biome(z,this, w));
        }
        noise = new TCODNoise(2, .5, 10,myRandom);
        tempNoise = new TCODNoise(2, .5, 10,tempRandom);
        altitudeNoise = new TCODNoise(2, .5, 10,altitudeRandom);
        humidityNoise = new TCODNoise(2, .5, 10,humidityRandom);
    }
    ~PerlinStoneNoiseGenerator() {
        delete myRandom;
        delete noise, tempNoise, altitudeNoise, humidityNoise;
    }
    void generateChunk(chunkId id, std::array<std::array<blockTypeId, CHUNK_Y_SIZE>, CHUNK_X_SIZE> *blocks,
                       WorldSystemInterface *worldSystemInterface) override;

    Biome *getBiome(std::tuple<uint16_t, uint16_t> tuple);
};


#endif //PRIZRAK_PERLINSTONENOISEGENERATOR_H
