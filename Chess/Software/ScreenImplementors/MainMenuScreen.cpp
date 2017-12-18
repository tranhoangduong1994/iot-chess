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

#include "KeyboardController.h"
#include "ScreenManager.h"

#include "ListScreen.h"

MainMenuScreen* MainMenuScreen::create() {
    MainMenuScreen* screen = new MainMenuScreen();
    return screen;
}

void MainMenuScreen::onEnter() {
    std::cout << "MainMenuScreen - onEnter" << std::endl;
    KeyboardController::getInstance()->setKeyboardEventsDelegator(this);
    
    if (!entered) {
        entered = true;
        cursorPositionIndex = 1;
        print(1, "MENU");	
        print(2, "->Single game");
        print(3, "  Multiplayer game");
        print(4, "  About");
    }
}

void MainMenuScreen::onExit() {
    std::cout << "MainMenuScreen - onExit" << std::endl;
    KeyboardController::getInstance()->setKeyboardEventsDelegator(NULL);
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
                ScreenManager::getInstance()->runScreen(GameSettingScreen::create());
                break;
            }
            case 2:
                break;
            case 3:
            {
                std::vector<std::string> entries;
                entries.push_back("____Duong H Tran____");
                entries.push_back("_____Ngoc Hoang_____");
                entries.push_back("______Gao Tran______");
                ScreenManager::getInstance()->pushScreen(ListScreen::create("_RoboticsChessboard_", entries));
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
            print(4, "  About");
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
            print(4, "->About");
            break;
        default:
            break;
    }

    cursorPositionIndex = index;
}
