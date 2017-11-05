//
//  Screen.cpp
//  Chess
//
//  Created by macbook on 11/5/17.
//  Copyright © 2017 Tran Hoang Duong. All rights reserved.
//

#include "Screen.h"

std::vector<Screen*> Screen::screenStack;
DisplayerProtocol* Screen::displayer = NULL;
Screen* Screen::currentScreen = NULL;

void Screen::pushScreen(Screen* screen) {
    if (currentScreen) {
        screenStack.push_back(currentScreen);
    }
    currentScreen = screen;
    displayer->clear();
    for (int i = 0; i < currentScreen->screenBuffer.size(); i++) {
        displayer->print(i, currentScreen->screenBuffer[i]);
    }
}

void Screen::popScreen() {
    if (screenStack.size() == 0) {
        return;
    }
    
    displayer->clear();
    
    delete screenStack.back();
    screenStack.pop_back();
    
    if (screenStack.size() > 0) {
        currentScreen = screenStack.back();
        for (int i = 0; i < currentScreen->screenBuffer.size(); i++) {
            displayer->print(i, currentScreen->screenBuffer[i]);
        }
    } else {
        currentScreen = NULL;
    }
}

void Screen::replaceScreen(Screen* screen) {
    if (screenStack.size() > 0) {
        delete screenStack.back();
        screenStack.pop_back();
    }
    
    displayer->clear();
    Screen::pushScreen(screen);
}

void Screen::print(int lineNumber, std::string content) {
    if (lineNumber > this->height) {
        return;
    }
    
    if (content.size() > this->width) {
        content = content.substr(this->width);
    }
    
    displayer->print(lineNumber, content);
}

void Screen::clearScreen() {
    displayer->clear();
}
