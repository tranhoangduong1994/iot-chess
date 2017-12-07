//
//  ListScreen.cpp
//  Chess
//
//  Created by macbook on 12/7/17.
//  Copyright © 2017 Tran Hoang Duong. All rights reserved.
//

#include "ListScreen.h"
#include "BoardServices.h"

ListScreen* ListScreen::create(std::string header, std::vector<std::string> items) {
    ListScreen* screen = new ListScreen();
    screen->header = header;
    screen->items = items;
    screen->init();
    return screen;
}

void ListScreen::init() {
    currentPage = 0;
}

void ListScreen::onEnter() {
    BoardServices::getInstance()->setBoardKeyEventsDelegate(this);
    print(1, header);
    updateScreen();
}

void ListScreen::onExit() {
    BoardServices::getInstance()->setBoardKeyEventsDelegate(NULL);
}

void ListScreen::updateScreen() {
//    int totalPage = items.size() / (SCREEN_HEIGHT - 1) + ((items.size() % SCREEN_HEIGHT - 1) ? 1 : 0);
    clear();
    for (int i = 1; i < SCREEN_HEIGHT; i++) {
        int itemNumber = currentPage * (SCREEN_HEIGHT - 1) + i;
        if (itemNumber > items.size()) {
            break;
        }
        print(i, items.at(itemNumber));
    }
}

void ListScreen::onKeyPressed(const KeyPressedData &data) {
    if (data.key == BoardKey::OK) {
        Screen::popScreen();
        closeCallback();
        return;
    }
    
    if (data.key == BoardKey::UP) {
        if (currentPage > 0) {
            currentPage--;
            updateScreen();
        }
        return;
    }
    
    if (data.key == BoardKey::DOWN) {
        int totalPage = items.size() / (SCREEN_HEIGHT - 1) + ((items.size() % SCREEN_HEIGHT - 1) ? 1 : 0);
        if (currentPage < totalPage) {
            currentPage++;
            updateScreen();
        }
        return;
    }
}
