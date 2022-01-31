//
// Created by justin on 10/23/21.
//

#ifndef PRIZRAK_RAWFILEFUNCTIONS_H
#define PRIZRAK_RAWFILEFUNCTIONS_H

#include <list>
#include "json.hpp"

using nlohmann::json;

std::list<std::pair<uint16_t , json>> loadRaws(const char *dir);

#endif //PRIZRAK_RAWFILEFUNCTIONS_H
