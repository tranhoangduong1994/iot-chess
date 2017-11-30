//
//  Screen.cpp
//  Chess
//
//  Created by macbook on 11/5/17.
//  Copyright © 2017 Tran Hoang Duong. All rights reserved.
//

#include "Screen.h"

std::vector<Screen*> Screen::screenStack;
IDisplayer* Screen::displayer = NULL;
Screen* Screen::currentScreen = NULL;

void Screen::runScreen(Screen* screen) {
    displayer->clear();
    screen->onEnter();
    if (screen) {
        for (int i = 0; i < currentScreen->screenBuffer.size(); i++) {
            displayer->print(i, currentScreen->screenBuffer[i]);
        }
    }
}

void Screen::pushScreen(Screen* screen) {
    if (currentScreen) {
        screenStack.push_back(currentScreen);
    }
    currentScreen = screen;
    runScreen(currentScreen);
}

void Screen::popScreen() {
    if (screenStack.size() == 0) {
        return;
    }
    
    screenStack.back()->onExit();
    //TODO:
    delete screenStack.back();
    screenStack.pop_back();
    
    if (screenStack.size() > 0) {
        currentScreen = screenStack.back();
        runScreen(currentScreen);
    } else {
        currentScreen = NULL;
        displayer->clear();
    }
}

void Screen::replaceScreen(Screen* screen) {
    if (screenStack.size() > 0) {
        screenStack.back()->onExit();
        //TODO: 
        delete screenStack.back();
        screenStack.pop_back();
    }
    
    Screen::pushScreen(screen);
}

void Screen::print(int lineNumber, std::string content) {
    
    if (lineNumber > SCREEN_HEIGHT) {
        return;
    }
    
    if (screenBuffer[lineNumber] == content) {
        return;
    }
    
    if (content.size() > SCREEN_WIDTH) {
        content = content.substr(SCREEN_WIDTH);
    }
    
    displayer->print(lineNumber, content);
}

void Screen::clearScreen() {
    displayer->clear();
}
