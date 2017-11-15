//
//  MainMenuScreen.cpp
//  Chess
//
//  Created by macbook on 11/6/17.
//  Copyright © 2017 Tran Hoang Duong. All rights reserved.
//

#include "MainMenuScreen.h"
#include "GameScreen.h"
#include "OfflineGame.h"
#include "BoardServices.h"

MainMenuScreen* MainMenuScreen::create(int width, int height) {
    if (!displayer) {
        displayer = DisplayerImplementation::getInstance();
    }
    
    MainMenuScreen* screen = new MainMenuScreen();
    screen->width = width;
    screen->height = height;
    
    return screen;
}

void MainMenuScreen::onEnter() {
    std::cout << "MainMenuScreen - onEnter" << std::endl;
    BoardServices::getInstance()->setBoardKeyEventsDelegate(this);
    setCursorPosition(1);
}

void MainMenuScreen::onExit() {
    std::cout << "MainMenuScreen - onExit" << std::endl;
    delete this;
}

void MainMenuScreen::onKeyPressed(const KeyPressedData& data) {
    BoardKey key = data.key;
    if (key == BoardKey::UP) {
        int index = cursorPositionIndex - 1;
        if (index == 0) {
            index = 3;
            setCursorPosition(index);
        }
    } else if (key == BoardKey::DOWN) {
        int index = cursorPositionIndex + 1;
        if (index == 4) {
            index = 1;
            setCursorPosition(index);
        }
    } else if (key == BoardKey::OK) {
        switch (cursorPositionIndex) {
            case 1:
            {
                OfflineGame* game = new OfflineGame();
                GameScreen* gameScreen = GameScreen::create(game, 20, 4);
                game->start(BaseTypes::WHITE, 1);
                Screen::replaceScreen(gameScreen);
                break;
            }
            case 2:
                break;
            case 3:
                break;
            default:
                break;
        }
    } else if (key == BoardKey::MENU) {
        
    }
}

void MainMenuScreen::init() {
    
}

void MainMenuScreen::setCursorPosition(int index) {
    displayer->print(1, "MENU");
    displayer->print(2, "  Single game");
    displayer->print(3, "  Multiplayer game");
    displayer->print(4, "  Settings");
    
    switch (index) {
        case 1:
            displayer->print(2, "* Single game");
            break;
        case 2:
            displayer->print(3, "* Multiplayer game");
            break;
        case 3:
            displayer->print(4, "* Settings");
            break;
        default:
            break;
    }
}
