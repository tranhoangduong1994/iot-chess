//
//  OffPositionScreen.cpp
//  Chess
//
//  Created by macbook on 12/8/17.
//  Copyright © 2017 Tran Hoang Duong. All rights reserved.
//

#include "OffPositionScreen.h"
#include "BoardServices.h"

OffPositionScreen* OffPositionScreen::create(const BaseTypes::Bitboard& currentState, const BaseTypes::Bitboard& expectedState) {
	BaseTypes::Bitboard offPiecePositions = expectedState.getOffPiecePositions(currentState);
    
    std::vector<std::string> items;
    int itemCount = 0;
    for (int i = 0; i < 64; i++) {
        if (offPiecePositions.get(i)) {
            itemCount++;
            std::string position = std::to_string(itemCount) + ". ";
            position += (char)((i % 8) + 97);
            position += std::to_string((i / 8) + 1);
            items.push_back(position);
        }
    }

    OffPositionScreen* screen = new OffPositionScreen("OFF POSITIONS", items);
    screen->expectedState = expectedState;
    return screen;
}

OffPositionScreen::OffPositionScreen(std::string header, std::vector<std::string> items) : ListScreen(header, items) {
}

void OffPositionScreen::onEnter() {
    ListScreen::onEnter();
    BoardServices::getInstance()->setBoardServicesEventsDelegate(this);
}

void OffPositionScreen::onExit() {
    BoardServices::getInstance()->setBoardServicesEventsDelegate(NULL);
    ListScreen::onExit();
}

void OffPositionScreen::onScanDone(BaseTypes::Bitboard currentPhysicsBitboard) {
    BaseTypes::Bitboard offPiecePositions = expectedState.getOffPiecePositions(currentPhysicsBitboard);
    
    std::vector<std::string> items;
    int itemCount = 0;
    for (int i = 0; i < 64; i++) {
        if (offPiecePositions.get(i)) {
            itemCount++;
            std::string position = std::to_string(itemCount) + ". ";
            position += (char)((i % 8) + 97);
            position += std::to_string((i / 8) + 1);
            items.push_back(position);
        }
    }
    
    if (items.size() > 0) {
		updateList(items);
		return;
	} 
	Screen::popScreen();
}

void OffPositionScreen::onMenuPressed() {
    
}

void OffPositionScreen::onOKPressed() {
	BoardServices::getInstance()->scan();
}
