//
// Created by Justin.Suess on 12/25/2021.
//

#ifndef PRIZRAK_GAMEVIEWFRAME_H
#define PRIZRAK_GAMEVIEWFRAME_H

#include "typenames.h"
#include "World.h"

class GameViewFrame {
private:
    World& world;
    TCODConsole& console;
    position p;
    instanceId  id;
    bool active = true;
    TCODMap* mymap;
    std::vector<std::string> logMessageBuffer;
public:

    explicit GameViewFrame(World& w, TCODConsole& con);

    void setSubject(instanceId id);

    instanceId getSubject();


    position getAbsolutePosition();

    void setAbsolutePosition(position p);

    void setUnderText(const char* s, const Appearance& a);

    void setOverText(const char* s, const Appearance& a);

    bool isGameViewActive();

    void setGameViewActive(bool active);

    void forceDrawToRoot();

    void printLogMinibar();
};


#endif //PRIZRAK_GAMEVIEWFRAME_H
