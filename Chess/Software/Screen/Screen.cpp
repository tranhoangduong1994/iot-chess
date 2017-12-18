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
	std::cout << "[Screen] print(" << std::to_string(lineNumber) << ", " << content << ")" << std::endl;
    Displayer* displayer = Displayer::getInstance();
    
    if (lineNumber > SCREEN_HEIGHT) {
        return;
    }
    
    if (content.size() > SCREEN_WIDTH) {
        content = content.substr(SCREEN_WIDTH);
    }
    
    //if (screenBuffer[lineNumber - 1] == content) {
        //return;
    //}
    
    screenBuffer[lineNumber - 1] = content;
    
    displayer->print(lineNumber, content);
}

void Screen::clear() {
	std::cout << "[Screen] clear()" << std::endl;
	Displayer* displayer = Displayer::getInstance();
	screenBuffer.clear();
    displayer->clear();
}

void Screen::refresh() {
    std::cout << "[Screen] refresh, buffer size = " << screenBuffer.size() << std::endl;
    for (int i = 0; i < screenBuffer.size(); i++) {
        std::cout << "[Screen] line " << i + 1 << " begin";
        std::cout << "[Screen] buffer " << i + 1 << ": " << screenBuffer.at(i) << std::endl;
        print(i + 1, screenBuffer.at(i));
        std::cout << "[Screen] line " << i + 1 << " end";
    }
}
