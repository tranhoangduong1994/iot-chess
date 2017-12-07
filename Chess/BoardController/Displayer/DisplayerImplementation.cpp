//
//  DisplayerImplementation.cpp
//  Chess
//
//  Created by macbook on 11/5/17.
//  Copyright © 2017 Tran Hoang Duong. All rights reserved.
//

#include "DisplayerImplementation.h"
#include "BoardServices.h"

const std::string CLEARING_STRING = "                    ";

DisplayerImplementation* DisplayerImplementation::instance = NULL;

DisplayerImplementation* DisplayerImplementation::getInstance() {
    if (!instance) {
        instance = new DisplayerImplementation();
    }
    return instance;
}

void DisplayerImplementation::print(int lineNumber, std::string content) {
    BoardServices::getInstance()->display(lineNumber, CLEARING_STRING);
    BoardServices::getInstance()->display(lineNumber, content);
}

void DisplayerImplementation::print(int lineNumber, int position, std::string content) {
    BoardServices::getInstance()->display(lineNumber, position, content);	
}

void DisplayerImplementation::clear() {
    BoardServices::getInstance()->clearScreen();
}
