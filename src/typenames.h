//
// Created by Justin.Suess on 10/4/2021.
//

#ifndef PRIZRAK_TYPENAMES_H
#define PRIZRAK_TYPENAMES_H

#include <stdint.h>
#include <tuple>

typedef unsigned char BYTE;

typedef uint32_t componentTypeId;
typedef uint64_t instanceId;
typedef uint16_t blockTypeId;

#define CHUNK_X_SIZE 64
#define CHUNK_Y_SIZE 64
#define CHUNK_Z_SIZE 32
#define LOADING_DISTANCE 8;
#define MAX_PATHFINDING_SIZE 1000
typedef uint32_t eventTypeId;
typedef uint32_t eventInstanceId;
typedef uint64_t componentInstanceId;
typedef uint64_t chunkId;
typedef uint16_t spriteId;
typedef std::tuple<uint16_t , uint16_t > position;

#include <climits>


#define DS(x) x = raw[#x]
#define SF(x) {#x, x}


#endif // PRIZRAK_TYPENAMES_H
