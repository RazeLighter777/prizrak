//
// Created by justin on 10/24/21.
//

#include "BlockType.h"

blockTypeId BlockType::getType() const {
    return id;
}

std::string BlockType::getName() {
    return name;
}

std::string BlockType::getDescription() {
    return desc;
}

bool BlockType::isSolid() {
    return solid;
}

bool BlockType::doGravity() {
    return gravity;
}

bool BlockType::isConcealing() {
    return concealing;
}

bool BlockType::isTransparent() {
    return transparent;
}

Appearance const &BlockType::getAppearance() {
    return a;
}

blockTypeId BlockType::getBrokenType() const {
    return brokenBlockId;
}
