//
// Created by Justin.Suess on 11/3/2021.
//

#ifndef PRIZRAK_UTIL_H
#define PRIZRAK_UTIL_H
#include <string>
#include <map>
#include <list>
#include "typenames.h"
double distance(position p1, position p2);
class GlobalGameState;
int randint(int min, int max);
void SwapBinary(uint64_t &value);
std::string substituteResources(uint32_t  input, const std::list<std::string>& subs, GlobalGameState& sub);

#define ID(x) stringHash(typeid(x).name())
#define NM(x) stringHash(x)
#endif //PRIZRAK_UTIL_H
