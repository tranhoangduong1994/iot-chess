//
//  OfflineGame.cpp
//  Chess
//
//  Created by macbook on 11/1/17.
//  Copyright © 2017 Tran Hoang Duong. All rights reserved.
//

#include "OfflineGame.h"
#include "GameEventsProtocol.h"
#include "StockfishEngine.h"
#include "PythonChessValidator.h"
#include "BoardServices.h"

#include "thread.h"

void OfflineGame::start(BaseTypes::Side side, int difficulty) {
    engine = StockfishEngine::getInstance();
    validator = PythonChessValidator::getInstance();
    
    this->difficulty = difficulty;
    
    engine->start(difficulty);
    validator->start();
    
    EventData gameStartedData;
    delegate->onGameStarted(gameStartedData);
    
    if (side == BaseTypes::Side::WHITE) {
        playerTurn();
    } else {
        computerTurn();
    }
}

void OfflineGame::playerTurn() {
    isPlayerTurn = true;
    EventData turnBeganData;
    if (moves.size() > 0) {
        BaseTypes::Move lastMove = moves.back();
        turnBeganData["opponent_move"] = lastMove.toString();
    }
    delegate->onTurnBegan(turnBeganData);
}

void OfflineGame::computerTurn() {
    isPlayerTurn = false;
    hasComputerFinishedThinking = false;
    engine->calculate([=](BaseTypes::Move move) {
        engine->move(move);
        validator->move(move);
        moves.push_back(move);
        BoardServices::getInstance()->move(move);
        hasComputerFinishedThinking = true;
    });
    Threads.main()->wait(hasComputerFinishedThinking);
}

void OfflineGame::setDelegate(GameEventsProtocol* delegate) {
    this->delegate = delegate;
}

void OfflineGame::onBoardStateChanged(const EventData& data) {
    if (!isPlayerTurn) {
        //TODO: handle INVALID ACTION
    } else {
        if (playerMovesFrom == "") {
            if (data.at("state") == "on") {
                //TODO: handle INVALID ACTION
                return;
            }
            
            playerMovesFrom = data.at("square");
        } else {
            if (data.at("state") == "off") {
                //TODO: handle INVALID ACTION
                return;
            }
            
            std::string playerMovesTo = data.at("square");
            std::string move = playerMovesFrom + playerMovesTo;
            playerMovesFrom = "";
            if (!validator->checkMove(move)) {
                //TODO: handle INVALID MOVE
                return;
            }
            
            validator->move(move);
            engine->move(move);
            moves.push_back(move);
            EventData data;
            delegate->onTurnEnded(data);
            computerTurn();
        }
    }
}

void OfflineGame::onMotorMoveDone() {
    playerTurn();
}

void OfflineGame::onGameReset() {
    engine->start(this->difficulty);
    validator->start();
    
    EventData gameStartedData;
    delegate->onGameStarted(gameStartedData);
}

void OfflineGame::onKeyPressed(const EventData& data) {
    
}
