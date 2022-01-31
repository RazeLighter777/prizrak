cmake_minimum_required( VERSION 3.0 )

## Required software
find_package( Threads REQUIRED )
## Sources and headers
include_directories( ${CMAKE_CURRENT_SOURCE_DIR} )
message(${PROJECT_SOURCE_DIR}/include)
## Find all source files to be compiled into object files
file( GLOB SOURCES *.cc *.cpp )


## C++ compiler options

## Specify a name for the generated executable file
set( MAIN_EXE_NAME prizrak_lib )
set(CMAKE_CXX_STANDARD 20)

option(BUILD_SHELL "build SQLite3 shell application" OFF)
##set(CMAKE_CXX_FLAGS "-O3 -std=c++20")

## 1. Compile...
add_library( ${MAIN_EXE_NAME}
        ${SOURCES}
        sqlite3.h sqlite3.c sqlite3ext.h Position.cpp Position.h  CollisionProperties.cpp CollisionProperties.h RawFileFunctions.cpp RawFileFunctions.h BlockType.cpp BlockType.h PerlinStoneNoiseGenerator.cpp PerlinStoneNoiseGenerator.h Appearance.h BaseCreatureStats.h BaseCreatureStateComponent.cpp BaseCreatureStateComponent.h IntellegentCreatureStats.cpp IntellegentCreatureStats.h IntellegentCreatureStateComponent.cpp IntellegentCreatureStateComponent.h Util.cpp GameViewFrame.cpp GameViewFrame.h GlobalGameState.cpp GlobalGameState.h MainMenuViewFrame.cpp MainMenuViewFrame.h InputCollector.cpp InputCollector.h SoundEngine.cpp SoundEngine.h InputHandlingComponent.cpp InputHandlingComponent.h InputGatheringSystem.cpp InputGatheringSystem.h PlayerComponent.cpp PlayerComponent.h KeyboardEvent.cpp KeyboardEvent.h  MovementEvent.cpp MovementEvent.h VoluntaryMovementHandler.cpp VoluntaryMovementHandler.h AppearanceComponent.cpp AppearanceComponent.h BasicAISystem.cpp BasicAISystem.h WorldEventDistributionAssistor.cpp WorldEventDistributionAssistor.h BreakBlockEvent.cpp BreakBlockEvent.h BlockBreakEventHandler.cpp BlockBreakEventHandler.h Biome.cpp Biome.h ItemArchetypeId.cpp ItemArchetypeId.h BasicItemProperties.cpp BasicItemProperties.h)


## 2. Link...
target_include_directories(${MAIN_EXE_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/include)
SET(LIBTCOD_LIBRARY libtcod.so)
if (WIN32)
    SET(LIBTCOD_LIBRARY ${PROJECT_SOURCE_DIR}/external/libtcod.a)
	SET(SFML_DIR "C:\\Users\\Justin.Suess\\Downloads\\SFML-2.5.1-sources\\SFML-2.5.1")
endif (WIN32)
find_package(SFML 2.5 COMPONENTS graphics audio)

target_link_libraries( ${MAIN_EXE_NAME}
        ${CMAKE_THREAD_LIBS_INIT}
        ${CMAKE_DL_LIBS}
        ${LIBTCOD_LIBRARY}
	sfml-graphics
	sfml-audio
        )


## 3. Install...
install( TARGETS ${MAIN_EXE_NAME}
        RUNTIME DESTINATION bin )
