//
// Created by Justin.Suess on 10/4/2021.
//

#ifndef PRIZRAK_HASHES_H
#define PRIZRAK_HASHES_H

uint32_t helper(uint32_t partial, const char *str);

uint32_t constexpr stringHash(char const *input) {
    return *input ?
           static_cast<unsigned int>(*input) + 33 * stringHash(input + 1) :
           5381;
}
#endif //PRIZRAK_HASHES_H
