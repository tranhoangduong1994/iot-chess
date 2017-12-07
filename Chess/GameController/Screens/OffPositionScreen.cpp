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

    OffPositionScreen* screen = static_cast<OffPositionScreen*>(ListScreen::create("OFF POSITIONS", items));
    screen->expectedState = expectedState;
    return screen;
}

void OffPositionScreen::onEnter() {
    ListScreen::onEnter();
    BoardServices::getInstance()->setBoardIngameEventsDelegate(this);
}

void OffPositionScreen::onExit() {
    BoardServices::getInstance()->setBoardIngameEventsDelegate(NULL);
    ListScreen::onExit();
}

void OffPositionScreen::onOpponentFinishedMove(const std::string &data, const std::string &newBoardState) {}

void OffPositionScreen::onBoardResetted() {}

void OffPositionScreen::onScanDone(const std::string &boardState) {
    BaseTypes::Bitboard newState(boardState);
    BaseTypes::Bitboard offPiecePositions = expectedState.getOffPiecePositions(newState);
    
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
    
    updateList(items);
}
