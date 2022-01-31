//
// Created by Justin.Suess on 12/25/2021.
//

#include <iostream>
#include <set>
#include "GameViewFrame.h"
#include "AppearanceComponent.h"
#include "BaseCreatureStateComponent.h"
#include "Util.h"
#include "EventListeningComponent.h"

GameViewFrame::GameViewFrame(World &w, TCODConsole& con) : world(w), console(con) {
    p = position({0,0});
    active = false;
    setSubject(0);
    forceDrawToRoot();
    int mapx = console.getWidth() - 1 - console.getWidth() / 4;
    int mapy = console.getHeight() - 2;
    mymap = new TCODMap( mapx , mapy);
}

position GameViewFrame::getAbsolutePosition() {
    return getSubject() == 0 ? p : world.getEntityById(getSubject()).getComponentOfType<Position>().value()->get();
}

void GameViewFrame::setAbsolutePosition(position pos) {
    p = pos;
}

void GameViewFrame::setUnderText(const char *s, const Appearance &a) {

}

void GameViewFrame::setOverText(const char *s, const Appearance &a) {

}

bool GameViewFrame::isGameViewActive() {
    return active;
}

void GameViewFrame::setGameViewActive(bool a) {
    active = a;
}

void GameViewFrame::forceDrawToRoot() {
    if (active) {
        console.clear();
        //allocate a frame.
        //draw blank screen if no subject
        if (getSubject() == 0 || !world.entityWithIdExists(getSubject())) {
            for (int x = console.getWidth() / 4; x < console.getWidth() - 1; x++) {
                for (int y = 1; y < console.getHeight() - 1; y++) {
                    console.setChar(x, y, '#');
                }
            }
        } else {
            position  p = getAbsolutePosition();
            //display stats
            console.print(0,0,std::string("X : ") + std::to_string(std::get<0>(p)) +  " Y : " + std::to_string(std::get<1>(p)));
            console.print(0, console.getHeight() / 8,  "HP: " + std::to_string(world.getEntityById(getSubject()).getComponentOfType<BaseCreatureStateComponent>().value()->bloodLevel));
            console.print(0, (2*console.getHeight()) / 8,  "Entity Count: " + std::to_string(world.getNumberOfLoadedEntities()));
            printLogMinibar();
            //compute the FOV.
            auto absx = std::get<0>(p) - (console.getWidth() - 1 - console.getWidth() / 4) / 2;
            int fovx = 0;
            int fovy = 0;
            std::set<chunkId> inView = {};
            for (int x = console.getWidth() / 4; x < console.getWidth() - 1; x++) {
                auto absy = std::get<1>(p) - (console.getHeight() - 2) / 2;
                fovy = 0;
                for (int y = 1; y < console.getHeight() - 1; y++) {
                    absy++;
                    fovy++;
                    if (!inView.contains(World::getChunkAtPosition({absx, absy}))) {
                        auto c = World::getChunkAtPosition({absx, absy});
                        world.generateOrLoad(c);
                        inView.insert(c);
                    }
                    bool val = !world.getBlockProperties(world.getBlockTypeAtPosition({absx, absy}))->isSolid();
                    mymap->setProperties(fovx,fovy, val, val);
                    //console.setChar(x, y, std::get<0>(world.getBlockProperties(world.getBlockTypeAtPosition({absx, absy}))->getAppearance().spriteIds.front()));
                }
                absx++;
                fovx++;
            }

            int centerx = (console.getWidth() - 1 - console.getWidth() / 4) / 2;
            int centery = (console.getHeight() - 2) / 2;
            mymap->computeFov(centerx, centery,60);
            //draw the map tiles
            absx = std::get<0>(p) - (console.getWidth() - 1 - console.getWidth() / 4) / 2;
            fovx = 0;
            fovy = 0;
            for (int x = console.getWidth() / 4; x < console.getWidth() - 1; x++) {
                auto absy = std::get<1>(p) - (console.getHeight() - 2) / 2;
                fovy = 0;
                for (int y = 1; y < console.getHeight() - 1; y++) {
                    absy++;
                    fovy++;
                    if (mymap->isInFov(fovx, fovy)) {
                        auto properties = world.getBlockProperties(world.getBlockTypeAtPosition({absx, absy}));
                        console.setChar(x, y, std::get<0>(properties->getAppearance().spriteIds.front()));
                        console.setCharForeground(x,y, std::get<1>(properties->getAppearance().spriteIds.front()));
                        //entities
                        auto entities = world.getEntitiesAtPosition({absx, absy});
                        if (!entities.empty()) {
                            //TODO: Scroll view between multiple entities
                            auto target = entities.front();
                            auto appearance = world.getEntityById(target).getComponentOfType<AppearanceComponent>();
                            if (appearance.has_value()) {
                                console.setChar(x, y, std::get<0>(appearance.value()->a.spriteIds.front()));
                                console.setCharForeground(x,y, std::get<1>(appearance.value()->a.spriteIds.front()));
                            }
                        }
                    }
                }
                absx++;
                fovx++;
            }
            for (auto c : world.getListOfLoadedChunks()) {
                if (distance(world.getPositionOfChunk(c), p) > 128) {
                    world.unloadChunk(c);
                }
            }

        }


        //TODO: Code to draw screen
        //draw the bottom text

        console.flush();

    }
}

void GameViewFrame::printLogMinibar() {
    auto& events = world.getEntityById(getSubject()).getComponentOfType<EventListeningComponent>().value()->currentEvents;
    if (!events.empty()) {
        for (Event* e : events) {
            logMessageBuffer.push_back(e->getStringRepresentation());
        }
    }
    if (logMessageBuffer.size() > 100) {
        logMessageBuffer.erase(logMessageBuffer.begin(),logMessageBuffer.begin() + 1);
    }
    for (int i = 1; i < 5 && i < logMessageBuffer.size(); i++) {
        console.print(0, console.getHeight() - i, logMessageBuffer[logMessageBuffer.size() - 1 - i]);
    }
}

void GameViewFrame::setSubject(instanceId id) {
    this->id = id;
}

instanceId GameViewFrame::getSubject() {
    return this->id;
}
