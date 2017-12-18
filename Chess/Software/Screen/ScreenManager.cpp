//
//  ScreenManager.cpp
//  Chess
//
//  Created by macbook on 12/18/17.
//  Copyright © 2017 Tran Hoang Duong. All rights reserved.
//

#include "ScreenManager.h"
#include "Displayer.h"

ScreenManager* ScreenManager::instance = NULL;

ScreenManager* ScreenManager::getInstance() {
    if (!instance) {
        instance = new ScreenManager();
        instance->init();
    }
    
    return instance;
}

void ScreenManager::runScreen(Screen* screen) {
    while (screenStack.size() > 0) {
        screenStack.back()->onExit();
        delete screenStack.back();
        screenStack.pop_back();
    }
    
    pushScreen(screen);
}

void ScreenManager::pushScreen(Screen* screen) {
    Displayer* displayer = Displayer::getInstance();
    screenStack.push_back(screen);
    displayer->clear();
    screen->onEnter();
    for (int i = 0; i < screen->screenBuffer.size(); i++) {
        displayer->print(i + 1, screen->screenBuffer[i]);
    }
}

void ScreenManager::popScreen() {
    Displayer* displayer = Displayer::getInstance();
    if (screenStack.size() == 0) {
        return;
    }
    
    screenStack.back()->onExit();
    delete screenStack.back();
    screenStack.pop_back();
    
    displayer->clear();
    
    if (screenStack.size() > 0) {
        Screen* currentScreen = screenStack.back();
        currentScreen->onEnter();
        for (int i = 0; i < currentScreen->screenBuffer.size(); i++) {
            displayer->print(i + 1, currentScreen->screenBuffer[i]);
        }
    }
}

void ScreenManager::init() {
    
}
