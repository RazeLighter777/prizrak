//
// Created by Justin.Suess on 11/3/2021.
//

#include "Util.h"

#include <cmath>
#include <cstdlib>
#include "GlobalGameState.h"
#include "hashes.h"
double distance(position p1, position p2) {
    return hypot(std::get<0>(p1) - std::get<0>(p2), std::get<1>(p1) - std::get<1>(p2));
}

int randint(int min, int max) {
    return min + ( std::rand() % ( max - min + 1 ) );
}
void SwapBinary(uint64_t &value) {
    value = ((value & 0x00000000FFFFFFFFull) << 32) | ((value & 0xFFFFFFFF00000000ull) >> 32);
    value = ((value & 0x0000FFFF0000FFFFull) << 16) | ((value & 0xFFFF0000FFFF0000ull) >> 16);
    value = ((value & 0x00FF00FF00FF00FFull) << 8)  | ((value & 0xFF00FF00FF00FF00ull) >> 8);
}


std::string substituteResources(uint32_t resourceId, const std::list<std::string>& subs, GlobalGameState& sub) {

    std::string res = sub.getResourceById(resourceId);
    unsigned i = 0;
    for (auto& s : subs) {
        const std::string index = "{" + std::to_string(i) + "}";
        while (true) {
            size_t pos = res.find(index);
            if (pos == std::string::npos) {
                break;
            }
            res.replace(pos, index.length(),s);
        }
        i++;
    }
    return res;
}