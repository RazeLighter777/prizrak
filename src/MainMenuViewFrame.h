//
// Created by Justin.Suess on 12/26/2021.
//

#ifndef PRIZRAK_MAINMENUVIEWFRAME_H
#define PRIZRAK_MAINMENUVIEWFRAME_H


#include <libtcod/console.hpp>

enum MAIN_MENU_CHOICE {
    START_GAME,
    EXIT,
    HELP,
};
class MainMenuViewFrame {
private:
    TCODConsole& console;
public:

    explicit MainMenuViewFrame(TCODConsole& con);

    MAIN_MENU_CHOICE runMenu();

};


#endif //PRIZRAK_MAINMENUVIEWFRAME_H
