//
// Created by Justin.Suess on 10/7/2021.
//

#include "Wuid.h"

uint64_t generateRandomWuid() {
    static std::mt19937_64 engine(std::random_device{}());
    static std::uniform_int_distribution<uint64_t> distribution;
    uint64_t ui64 = distribution(engine);
    return ui64;
}
