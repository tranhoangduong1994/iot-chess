//
//  ListScreen.cpp
//  Chess
//
//  Created by macbook on 12/7/17.
//  Copyright © 2017 Tran Hoang Duong. All rights reserved.
//

#include "ListScreen.h"
#include "BoardServices.h"
#include "KeyboardController.h"
#include "ScreenManager.h"

ListScreen* ListScreen::create(std::string header, std::vector<std::string> items) {
    ListScreen* screen = new ListScreen();
    screen->header = header;
    screen->items = items;
    screen->init();
    return screen;
}

ListScreen::ListScreen(std::string header, std::vector<std::string> items) {
	this->header = header;
	this->items = items;
	init();
}

void ListScreen::init() {
    currentPage = 0;
}

void ListScreen::onEnter() {
    KeyboardController::getInstance()->setKeyboardEventsDelegator(this);
    print(1, header);
    updateScreen();
}

void ListScreen::onExit() {
    KeyboardController::getInstance()->setKeyboardEventsDelegator(NULL);
}

void ListScreen::updateScreen() {
    for (int i = 0; i < SCREEN_HEIGHT; i++) {
		std::cout << "i = " << i << std::endl;
        int itemNumber = currentPage * (SCREEN_HEIGHT - 1) + i;
        std::string string = "";
        if (itemNumber < items.size()) {
            string = items.at(itemNumber);
        }
        print(i + 2, string);
    }
}

void ListScreen::updateList(const std::vector<std::string>& items) {
    this->items = items;
    currentPage = 0;
    updateScreen();
}

void ListScreen::onKeyPressed(BoardKey key) {
    if (key == BoardKey::MENU) {
        onMenuPressed();
        return;
    }
    
    if (key == BoardKey::OK) {
		onOKPressed();
		return;
    }
    
    if (key == BoardKey::UP) {
		onUpPressed();
        return;
    }
    
    if (key == BoardKey::DOWN) {
        onDownPressed();
        return;
    }
}

void ListScreen::onMenuPressed() {}

void ListScreen::onOKPressed() {
    ScreenManager::getInstance()->popScreen();
}

void ListScreen::onUpPressed() {
	if (currentPage > 0) {
		currentPage--;
		updateScreen();
	}
}

void ListScreen::onDownPressed() {
	int totalPage = items.size() / (SCREEN_HEIGHT - 1) + ((items.size() % SCREEN_HEIGHT - 1) ? 1 : 0);
	if (currentPage < totalPage) {
		currentPage++;
		updateScreen();
	}
}
