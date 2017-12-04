//
//  DisplayerImplementation.cpp
//  Chess
//
//  Created by macbook on 11/5/17.
//  Copyright © 2017 Tran Hoang Duong. All rights reserved.
//

#include "DisplayerImplementation.h"
#include "BoardServices.h"

DisplayerImplementation* DisplayerImplementation::instance = NULL;

DisplayerImplementation* DisplayerImplementation::getInstance() {
    if (!instance) {
        instance = new DisplayerImplementation();
    }
    return instance;
}

void DisplayerImplementation::print(int lineNumber, std::string content) {
	std::cout << "[DisplayerImplementation] print(" << lineNumber << ", " << content << ")" << std::endl;
    BoardServices::getInstance()->display(lineNumber, content);
}

void DisplayerImplementation::print(int lineNumber, int position, std::string content) {
	std::cout << "[DisplayerImplementation] print(" << lineNumber << ", " << content << ")" << std::endl;
    BoardServices::getInstance()->display(lineNumber, position, content);	
}

void DisplayerImplementation::clear() {
    BoardServices::getInstance()->clearScreen();
}
