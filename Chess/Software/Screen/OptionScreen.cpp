//
//  OptionScreen.cpp
//  Chess
//
//  Created by macbook on 11/29/17.
//  Copyright © 2017 Tran Hoang Duong. All rights reserved.
//

#include "OptionScreen.h"

#include "BoardServices.h"

#include "KeyboardController.h"

#include <thread>

OptionScreen* OptionScreen::create(std::string header, std::vector<OptionScreenEntry> entries) {    
    OptionScreen* screen = new OptionScreen();
    screen->header = header;
    screen->entries = entries;
    screen->init();
    
    return screen;
}

OptionScreen::OptionScreen(std::string header, std::vector<OptionScreenEntry> entries) {
    this->header = header;
    this->entries = entries;
    init();
}

void OptionScreen::init() {
    cursorPositionIndex = 0;
    minEntryIndex = 0;
    maxEntryIndex = minEntryIndex + (SCREEN_HEIGHT - 2);
    maxEntryIndex = maxEntryIndex < entries.size() - 1 ? maxEntryIndex : entries.size() - 1;
}

void OptionScreen::onEnter() {
    KeyboardController::getInstance()->setKeyboardEventsDelegator(this);
    print(1, header);
    updateScreen();
}

void OptionScreen::onExit() {
    KeyboardController::getInstance()->setKeyboardEventsDelegator(NULL);
}

void OptionScreen::updateScreen() {
    int iMax = maxEntryIndex < entries.size() - 1 ? maxEntryIndex : entries.size() - 1;
    for (int i = minEntryIndex; i <= iMax; i++) {
		std::string printedString;
        if (i == cursorPositionIndex) {
			printedString = "* " + entries.at(i).name;
        } else {
			printedString = "  " + entries.at(i).name;
        }
			
		if (0 < i && i < entries.size() - 1) {
			if (i == minEntryIndex) {
				printedString += "<";
			} else if (i == iMax) {
				printedString += ">";
			}
		}
		print(i - minEntryIndex + 2, printedString);
    }
}

void OptionScreen::onKeyPressed(BoardKey key) {
    if (key == BoardKey::OK) {
        OptionScreenEntry& entry = entries.at(cursorPositionIndex);
        entry.onSelected(entry.name);
        return;
    }
    
    if (key == BoardKey::DOWN) {
        cursorPositionIndex++;
        if (cursorPositionIndex >= entries.size() - 1) {
            cursorPositionIndex = entries.size() - 1;
        }
        
        if (cursorPositionIndex > maxEntryIndex) {
            minEntryIndex += 1;
            maxEntryIndex += 1;
        }
        
        updateScreen();
        return;
    }
    
    if (key == BoardKey::UP) {
        cursorPositionIndex--;
        if (cursorPositionIndex <= -1) {
            cursorPositionIndex = 0;
        }
        
        if (cursorPositionIndex < minEntryIndex) {
            minEntryIndex -= 1;
            maxEntryIndex -= 1;
        }
        
        updateScreen();
        return;
    }
}


