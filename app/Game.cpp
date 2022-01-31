//
// Created by Justin.Suess on 10/28/2021.
//

#include <iostream>
#include <chrono>
#include <thread>
#include <GlobalGameState.h>
#include <ItemArchetypeId.h>
#include <MainMenuViewFrame.h>
#include <GameViewFrame.h>
#include <AppearanceComponent.h>
#include <BaseCreatureStateComponent.h>
#include <BasicAISystem.h>
#include <EventListeningComponent.h>
#include <BlockBreakEventHandler.h>

#include "PerlinStoneNoiseGenerator.h"
#include "SoundEngine.h"
#include "Game.h"
#include "InputGatheringSystem.h"
#include "World.h"
#include "typenames.h"
#include "BreakBlockEvent.h"
#include "RawFileFunctions.h"
#include "CreatureArchetypeId.h"
#include "VoluntaryMovementHandler.h"
#include "json.hpp"
#include "MovementEvent.h"
#include "Util.h"
#include "PlayerComponent.h"
Game* Game::instance = nullptr;

[[noreturn ]] void Game::runGame() {
    TCODConsole::initRoot(90, 50, "Prizrak 0.0.0 DEV BUILD",false,TCOD_RENDERER_OPENGL2);

    GlobalGameState g = GlobalGameState::getInstance();
    int ysize = TCODConsole::root->getHeight();
    unsigned int xpos = 4343;
    unsigned int ypos = 12341234;
    MainMenuViewFrame m = MainMenuViewFrame(*TCODConsole::root);
    //TCODConsole::root->setCustomFont("./terminal.png", TCOD_FONT_LAYOUT_ASCII_INROW, 32, 8);
    auto menu = m.runMenu();
    if (menu == START_GAME) {
        auto raws = loadRaws("../raws");
        World w = World();
        //TODO: Allow player to load world and raws
        w.loadRaws(raws);
        w.registerBlockType(new BlockType(w.getRaw(stringHash("block/stone"))));
        w.registerBlockType(new BlockType(w.getRaw(stringHash("block/stonefloor"))));
        w.registerBlockType(new BlockType(w.getRaw(stringHash("block/sand"))));
        w.setWorldGenerator(new PerlinStoneNoiseGenerator(&w));
        GameViewFrame g(w, *TCODConsole::root);
        //TODO: Character creation screen
        //register all of the components
        w.registerComponentDeserializationFunction<BaseCreatureStateComponent>(BaseCreatureStateComponent::deserialize);
        w.registerComponentDeserializationFunction<CreatureArchetypeId>(CreatureArchetypeId::deserialize);
        w.registerComponentDeserializationFunction<ItemArchetypeId>(ItemArchetypeId::deserialize);
        w.registerComponentDeserializationFunction<Position>(Position::deserialize);
        auto ptid = w.registerComponentDeserializationFunction<PlayerComponent>(PlayerComponent::deserialize);
        w.registerComponentDeserializationFunction<AppearanceComponent>(AppearanceComponent::deserialize);
        w.registerComponentDeserializationFunction<EventListeningComponent>(EventListeningComponent::deserialize);
        w.registerComponentDeserializationFunction<BaseCreatureStateComponent>(BaseCreatureStateComponent::deserialize);
        //register all systems.
        w.registerEventHandlingSystem(ID(MovementEvent),new VoluntaryMovementHandler(&w));
        w.registerEventHandlingSystem(ID(BreakBlockEvent), new BlockBreakEventHandler(&w));
        w.registerSystem(new InputGatheringSystem());
        w.registerSystem(new BasicAISystem());
        //spawn le epic lizard if not in game
        w.loadWorldFromDatabase("test.db");
        g.setSubject(GlobalGameState::getInstance().getPlayerInstance());
        if (g.getSubject() == 0) {
            auto iid = generateRandomWuid();
            CreatureArchetypeId::createCreatureInWorld(&w, iid,"creature/gecko",{32000,32000});
            w.addComponentToEntity(w.deserializeComponent({
                                           {"tid", ptid},
                                           {"iid", iid},
                                           {"id", generateRandomWuid()},
                                           {"dat", {
                                               "player",true
                                           }}
            }).first, iid);
            g.setSubject(iid);
        }
        g.setGameViewActive(true);
        //CreatureArchetypeId::createCreatureInWorld(&w, generateRandomWuid(),"creature/lizard",{32001,32000});
        short tick = 0;
        while (GlobalGameState::getInstance().getState() != QUIT) {
            w.runTick();
            g.forceDrawToRoot();
            w.deleteOldEvents();

            if (tick % 250 == 0) {
                w.saveWorld();
            }
            tick++;
        }
    }
    else if (menu == EXIT) {
        return;
    }
}

void Game::loadStandardRawsAndInitializeComponents(World *w) {
    auto raws = loadRaws("../raws");
    w->loadRaws(raws);
    w->registerBlockType(new BlockType(w->getRaw(stringHash("block/stone"))));
    //CreatureArchetypeId::addRaw(0, w->getRaw("creature/lizard"));
}
