//
// Created by justin on 10/24/21.
//

#ifndef PRIZRAK_BLOCKTYPE_H
#define PRIZRAK_BLOCKTYPE_H

#include <string>
#include "json.hpp"
#include "typenames.h"
#include "hashes.h"
#include "Appearance.h"

using nlohmann::json;

class BlockType {
private:
    blockTypeId id;
    blockTypeId brokenBlockId;
    std::string name;
    std::string desc;
    bool solid;
    bool gravity;
    bool transparent;
    bool concealing;
    Appearance a;
public:
    explicit BlockType(json raw) : a() {
        a.animation = STATIC;
        a.layer = GROUND;
        if (raw.contains("code")) {
            const spriteId i = raw["code"].get<int>();

            a.spriteIds.push_back({i, TCOD_color_RGB(raw["fg"]["r"], raw["fg"]["g"], raw["fg"]["b"]), {255, 0, 255}});
        } else {
            a.spriteIds.push_back({47, {255, 0, 255}, {255, 0, 255}});
        }
        id = stringHash((std::string("block/") + raw["name"].get<std::string>()).c_str());
        name = raw["name"];
        desc = raw["description"];
        solid = raw["solid"];
        if (solid) {
            brokenBlockId = stringHash(raw["brokenBlock"].get<std::string>().c_str());
        }
        gravity = raw["gravity"];
        transparent = raw["transparent"];
        concealing = raw["concealing"];
    }

    blockTypeId getType() const;

    blockTypeId getBrokenType() const;

    std::string getName();

    std::string getDescription();

    bool isSolid();

    bool isConcealing();

    bool isTransparent();

    bool doGravity();

    Appearance const &getAppearance();

    //virtual RenderInfo getRenderingInfo() = 0;
};


#endif //PRIZRAK_BLOCKTYPE_H
