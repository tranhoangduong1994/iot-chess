//
//  Screen.cpp
//  Chess
//
//  Created by macbook on 11/5/17.
//  Copyright © 2017 Tran Hoang Duong. All rights reserved.
//

#include "Screen.h"

#include <iostream>

#include "DisplayerImplementation.h"

std::vector<Screen*> Screen::screenStack;

Screen::Screen() {
	screenBuffer.resize(SCREEN_HEIGHT);
}

void Screen::runScreen(Screen* screen) {
	if (screenStack.size() > 0) {
		screenStack.back()->onExit();
		delete screenStack.back();
		screenStack.pop_back();
	}
	
	pushScreen(screen);
}

void Screen::pushScreen(Screen* screen) {
	IDisplayer* displayer = DisplayerImplementation::getInstance();
	screenStack.push_back(screen);
	displayer->clear();
	screen->onEnter();
	for (int i = 0; i < screen->screenBuffer.size(); i++) {
		displayer->print(i + 1, screen->screenBuffer[i]);
	}
}

void Screen::popScreen() {
	IDisplayer* displayer = DisplayerImplementation::getInstance();
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

void Screen::print(int lineNumber, std::string content) {
	std::cout << "[Screen] print(" << std::to_string(lineNumber) << ", " << content << ")" << std::endl;
    IDisplayer* displayer = DisplayerImplementation::getInstance();
    
    if (lineNumber > SCREEN_HEIGHT) {
        return;
    }
    
    if (content.size() > SCREEN_WIDTH) {
        content = content.substr(SCREEN_WIDTH);
    }
    
    if (screenBuffer[lineNumber - 1] == content) {
        return;
    }
    
    screenBuffer[lineNumber - 1] = content;
    
    displayer->print(lineNumber, content);
}

void Screen::clear() {
	std::cout << "[Screen] clear()" << std::endl;
	IDisplayer* displayer = DisplayerImplementation::getInstance();
	screenBuffer.clear();
    displayer->clear();
}
