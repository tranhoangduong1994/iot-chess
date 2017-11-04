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

#include "thread.h"

void OfflineGame::start(BaseTypes::Side side, int difficulty) {
    engine = StockfishEngine::getInstance();
    validator = PythonChessValidator::getInstance();
    
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

void OfflineGame::move(BaseTypes::Move move) {
    if (validator->checkMove(move)) {
        validator->move(move);
        engine->move(move);
        EventData turnEndedData;
        turnEndedData["your_move"] = move.toString();
        delegate->onTurnEnded(turnEndedData);
        computerTurn();
    } else {
        EventData invalidMoveData;
        invalidMoveData["your_move"] = move.toString();
        delegate->onInvalidMove(invalidMoveData);
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
        hasComputerFinishedThinking = true;
    });
    Threads.main()->wait(hasComputerFinishedThinking);
    playerTurn();
}

void OfflineGame::setDelegate(GameEventsProtocol* delegate) {
    this->delegate = delegate;
}
