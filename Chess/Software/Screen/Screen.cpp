//
//  Screen.cpp
//  Chess
//
//  Created by macbook on 11/5/17.
//  Copyright © 2017 Tran Hoang Duong. All rights reserved.
//

#include "Screen.h"

#include <iostream>

#include "Displayer.h"

Screen::Screen() {
	screenBuffer.resize(SCREEN_HEIGHT);
    for (int i = 0; i < screenBuffer.size(); i++) {
        screenBuffer[i] = "";
    }
    
}

void Screen::print(int lineNumber, std::string content) {
    if (lineNumber > SCREEN_HEIGHT) {
        return;
    }
    
    if (content.size() > SCREEN_WIDTH) {
        content = content.substr(SCREEN_WIDTH);
    }
    
    screenBuffer[lineNumber - 1] = content;
    
    Displayer::getInstance()->print(lineNumber, content);
}

void Screen::clear() {
	screenBuffer.clear();
    Displayer::getInstance()->clear();
}

void Screen::refresh() {
    for (int i = 0; i < screenBuffer.size(); i++) {
        print(i + 1, screenBuffer.at(i));
    }
}
