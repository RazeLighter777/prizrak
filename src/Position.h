//
// Created by Justin.Suess on 10/16/2021.
//

#ifndef PRIZRAK_POSITION_H
#define PRIZRAK_POSITION_H

#include <utility>

#include "Component.h"
#include "WorldSystemInterface.h"

class Position final : public Component {
private:
    WorldSystemInterface *w;
    position internal;
    instanceId iid;
public:
    Position(WorldSystemInterface *w, position internal, instanceId iid);

    static Component *deserialize(json& j, WorldSystemInterface *w, instanceId iid);

    json serializationFunction(void) override;

    void operator=(position other);

    void operator=(const Position &other);

    position get() const;

    ~Position() override;
};


#endif //PRIZRAK_POSITION_H
