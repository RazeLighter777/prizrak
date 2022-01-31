//
// Created by Justin.Suess on 12/26/2021.
//

#include <optional>
#include <chrono>
#include <thread>
#include "hashes.h"
#include "MainMenuViewFrame.h"
#include "GlobalGameState.h"
#include "Util.h"

MainMenuViewFrame::MainMenuViewFrame(TCODConsole &con) : console(con) {

}

MAIN_MENU_CHOICE MainMenuViewFrame::runMenu() {
    console.credits();
    GlobalGameState::getInstance().getSound().setMusic(NM("main_menu.ogg"));
    while (true) {
        for (int i = 0; i < 50; i++) {
            int x = rand() % console.getWidth();
            int y = rand() % console.getHeight();
            console.setChar(x, y, rand() % 255);
            console.setCharForeground(x, y, TCOD_dark_grey);
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        console.print(console.getWidth() / 2 - 1, 0, "PRIZRAK");
        console.print( 4, console.getHeight() / 4, " 1 :-: BEGIN GAME");
        console.print( 4, console.getHeight() / 2, " 2 :-: HELP MENU");
        console.print( 4, (3 * console.getHeight()) / 4, " 3 :-: EXIT");

        console.flush();
        auto key = GlobalGameState::getInstance().getInput().nonBlockingKeyboardInput().vk;
        if (key == TCODK_1) {
            return START_GAME;
        }  else if (key == TCODK_ESCAPE || key == TCODK_3) {
            return EXIT;
        }

        console.clear();
    }
    return START_GAME;
}
