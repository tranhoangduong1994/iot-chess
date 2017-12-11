//
//  MainMenuScreen.cpp
//  Chess
//
//  Created by macbook on 11/6/17.
//  Copyright © 2017 Tran Hoang Duong. All rights reserved.
//

#include "MainMenuScreen.h"
#include "GameSettingScreen.h"
#include "BoardServices.h"

#include "OptionScreen.h"

MainMenuScreen* MainMenuScreen::create() {
    MainMenuScreen* screen = new MainMenuScreen();
    return screen;
}

void MainMenuScreen::onEnter() {
    std::cout << "MainMenuScreen - onEnter" << std::endl;
    BoardServices::getInstance()->setBoardKeyEventsDelegate(this);
    
    if (!entered) {
        entered = true;
        cursorPositionIndex = 1;
        print(1, "MENU");	
        print(2, "->Single game");
        print(3, "  Multiplayer game");
        print(4, "  Settings");
    }
}

void MainMenuScreen::onExit() {
    std::cout << "MainMenuScreen - onExit" << std::endl;
    BoardServices::getInstance()->setBoardKeyEventsDelegate(NULL);
}

void MainMenuScreen::onKeyPressed(BoardKey key) {
    if (key == BoardKey::UP) {
        int index = cursorPositionIndex - 1;
        if (index == 0) {
            index = 3;
        }
        //cursorPositionIndex = index;
        setCursorPosition(index);
    } else if (key == BoardKey::DOWN) {
        int index = cursorPositionIndex + 1;
        if (index == 4) {
            index = 1;
        }
        //cursorPositionIndex = index;
        setCursorPosition(index);
    } else if (key == BoardKey::OK) {
        switch (cursorPositionIndex) {
            case 1:
            {
                Screen::runScreen(GameSettingScreen::create());
                break;
            }
            case 2:
                break;
            case 3:
            {
                std::vector<OptionScreenEntry> entries;
                for (int i = 0; i < 20; i++) {
                    OptionScreenEntry entry;
                    entry.name = "Test " + std::to_string(i);
                    entry.onSelected = [=](std::string content) {
                        std::cout << "You selected: " + content << std::endl;
                    };
                    entries.push_back(entry);
                }
                OptionScreenEntry cancelEntry;
                cancelEntry.name = "CANCEL";
                cancelEntry.onSelected = [=](std::string content) {
                    std::cout << "Cancelled";
                };
                entries.push_back(cancelEntry);

                OptionScreen* screen = OptionScreen::create("Test option screen", entries);
                Screen::pushScreen(screen);
                break;
            }
            default:
                break;
        }
    } else if (key == BoardKey::MENU) {
		
    }
}

void MainMenuScreen::init() {
    entered = false;
}

void MainMenuScreen::setCursorPosition(int index) {
    print(1, "MENU");

    switch(cursorPositionIndex) {
        case 1:
            print(2, "  Single game");
            break;
        case 2:
            print(3, "  Multiplayer game");
            break;
        case 3:
            print(4, "  Settings");
            break;	
    }

    switch (index) {
        case 1:		
            print(2, "->Single game");
            break;
        case 2:
            print(3, "->Multiplayer game");
            break;
        case 3:
            print(4, "->Settings");
            break;
        default:
            break;
    }

    cursorPositionIndex = index;
}
