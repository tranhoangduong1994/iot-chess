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
    
    if (lineNumber > SCREEN_HEIGHT) {
        std::cout << "[Screen] print - 1" << std::endl;
        return;
    }
    
    if (content.size() > SCREEN_WIDTH) {
        std::cout << "[Screen] print - 2" << std::endl;
        content = content.substr(SCREEN_WIDTH);
    }
    
    //if (screenBuffer[lineNumber - 1] == content) {
        //return;
    //}
    
    std::cout << "[Screen] print - 3" << std::endl;
    
    screenBuffer[lineNumber - 1] = content;
    
    std::cout << "[Screen] print - 4" << std::endl;
    
    Displayer::getInstance()->print(lineNumber, content);
    
    std::cout << "[Screen] print - 5" << std::endl;
}

void Screen::clear() {
	std::cout << "[Screen] clear()" << std::endl;
	screenBuffer.clear();
    Displayer::getInstance()->clear();
}

void Screen::refresh() {
    std::cout << "[Screen] refresh, buffer size = " << screenBuffer.size() << std::endl;
    for (int i = 0; i < screenBuffer.size(); i++) {
        std::cout << "[Screen] line " << i + 1 << " begin" << std::endl;
        std::cout << "[Screen] buffer " << i + 1 << ": " << screenBuffer.at(i) << std::endl;
        print(i + 1, screenBuffer.at(i));
        std::cout << "[Screen] line " << i + 1 << " end" << std::endl;
    }
}
