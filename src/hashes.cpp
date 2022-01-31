//
// Created by Justin.Suess on 10/4/2021.
//

#include <cstdint>
#include "hashes.h"

static constexpr uint32_t offset = 2166136261u;
static constexpr uint32_t prime = 16777619u;

uint32_t helper(uint32_t partial, const char *str) {
    return str[0] == 0 ? partial : helper((partial ^ str[0]) * prime, str + 1);
}

/*

uint32_t stringHash(const char *input) {
    return helper(offset, input);
}*/
