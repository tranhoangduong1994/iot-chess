//
//  OptionScreen.cpp
//  Chess
//
//  Created by macbook on 11/29/17.
//  Copyright © 2017 Tran Hoang Duong. All rights reserved.
//

#include "OptionScreen.h"

#include "BoardServices.h"

OptionScreen* OptionScreen::create(std::string header, std::vector<OptionScreenEntry> entries) {
    if (!displayer) {
        displayer = DisplayerImplementation::getInstance();
    }
    
    OptionScreen* screen = new OptionScreen();
    screen->entries = entries;
    screen->init();
    
    return screen;
}

void OptionScreen::init() {
    cursorPositionIndex = 0;
    minEntryIndex = 0;
    maxEntryIndex = minEntryIndex + (SCREEN_HEIGHT - 2);
    maxEntryIndex = maxEntryIndex < entries.size() - 1 ? maxEntryIndex : entries.size() - 1;
}

void OptionScreen::onEnter() {
    BoardServices::getInstance()->setBoardKeyEventsDelegate(this);
}

void OptionScreen::onExit() {
    BoardServices::getInstance()->setBoardKeyEventsDelegate(NULL);
}

void OptionScreen::updateScreen() {
    int iMax = maxEntryIndex < entries.size() - 1 ? maxEntryIndex : entries.size() - 1;
    for (int i = minEntryIndex; i <= iMax; i++) {
        if (i == cursorPositionIndex) {
            print(i - minEntryIndex + 2, "* " + entries.at(i).name);
        } else {
            print(i - minEntryIndex + 2, "  " + entries.at(i).name);
        }
    }
}

void OptionScreen::onKeyPressed(const KeyPressedData& data) {
    if (data.key == BoardKey::OK) {
        OptionScreenEntry& entry = entries.at(cursorPositionIndex);
//        entry.onSelected(BaseTypes::Move(entry.name));
        entry.onSelected(entry.name);
        Screen::popScreen();
    }
    
    if (data.key == BoardKey::DOWN) {
        cursorPositionIndex++;
        if (cursorPositionIndex >= entries.size() - 1) {
            cursorPositionIndex = entries.size() - 1;
        }
        
        if (cursorPositionIndex > maxEntryIndex) {
            minEntryIndex += 1;
            maxEntryIndex += 1;
        }
        
        updateScreen();
    }
    
    if (data.key == BoardKey::UP) {
        cursorPositionIndex--;
        if (cursorPositionIndex <= -1) {
            cursorPositionIndex = 0;
        }
        
        if (cursorPositionIndex < minEntryIndex) {
            minEntryIndex -= 1;
            maxEntryIndex -= 1;
        }
        
        updateScreen();
    }
}


