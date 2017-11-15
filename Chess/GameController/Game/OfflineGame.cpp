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

const std::string BOARD[] = {
    "A1", "A2", "A3", "A4", "A5", "A6", "A7", "A8",
    "B1", "B2", "B3", "B4", "B5", "B6", "B7", "B8",
    "C1", "C2", "C3", "C4", "C5", "C6", "C7", "C8",
    "D1", "D2", "D3", "D4", "D5", "D6", "D7", "D8",
    "E1", "E2", "E3", "E4", "E5", "E6", "E7", "E8",
    "F1", "F2", "F3", "F4", "F5", "F6", "F7", "F8",
    "G1", "G2", "G3", "G4", "G5", "G6", "G7", "G8",
    "H1", "H2", "H3", "H4", "H5", "H6", "H7", "H8",
};

const std::string BOARD_INIT_STATE = "1111111111111111000000000000000000000000000000001111111111111111";

void OfflineGame::validateInitState() {
    if (initValidatingState != InitValidatingState::NOT_VALIDATED) return;
    initValidatingState = InitValidatingState::VALIDATED;
    BoardServices::getInstance()->scan();
}

std::vector<BaseTypes::Position> OfflineGame::getBoardStateMisplacedPositions(const std::string& boardState) {
    std::vector<BaseTypes::Position> misplacedPositions;
    if (boardState != BOARD_INIT_STATE) {
        std::vector<BaseTypes::Position> misplacedPositions;
        for (int i = 0; i < BOARD_INIT_STATE.size(); i++) {
            if (BOARD_INIT_STATE.at(i) == '1' && boardState.at(i) == '0') {
                misplacedPositions.push_back(BaseTypes::Position(BOARD[i]));
            }
        }
    }
    return misplacedPositions;
}

void OfflineGame::start(BaseTypes::Side side, int difficulty) {
    engine = StockfishEngine::getInstance();
    validator = PythonChessValidator::getInstance();
    
    this->side = side;
    this->difficulty = difficulty;
    
    engine->start(difficulty);
    validator->start();
    
    initValidatingState = InitValidatingState::NOT_VALIDATED;
    validateInitState();
}

void OfflineGame::startPlayerTurn() {
    isPlayerTurn = true;
    TurnBeganData data;
    if (moves.size() > 0) {
        BaseTypes::Move lastMove = moves.back();
        data.opponent_move = lastMove.toString();
    }
    delegate->onTurnBegan(data);
}

void OfflineGame::startOpponentTurn() {
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

void OfflineGame::onPlayerFinishedMove(const std::string& data) {
    if (validator->checkDraw()) {
        delegate->onDrawGame(data);
        return;
    }
    if (validator->checkGameOver()) {
        delegate->onWinGame(data);
        return;
    }
    startPlayerTurn();
}

void OfflineGame::onOpponentFinishedMove(const std::string& data) {
    if (validator->checkDraw()) {
        delegate->onDrawGame(data);
        return;
    }
    if (validator->checkGameOver()) {
        delegate->onLoseGame(data);
        return;
    }
    startPlayerTurn();
}

void OfflineGame::onBoardResetted() {
    engine->start(this->difficulty);
    validator->start();
    
    delegate->onGameStarted(GameStartedData(side));
    
    if (side == BaseTypes::Side::WHITE) {
        startPlayerTurn();
    } else {
        startOpponentTurn();
    }
}

void OfflineGame::onKeyPressed(const KeyPressedData& data) {
    
}

void OfflineGame::onPlayerChangedBoardState(const std::string &boardState) {
    if (initValidatingState == InitValidatingState::VALIDATING) {
        onScanDone(boardState);
    }
}

void OfflineGame::onScanDone(const std::string& boardState) {
    if (initValidatingState == InitValidatingState::VALIDATING) {
        const std::vector<BaseTypes::Position>& misplacedPositions = getBoardStateMisplacedPositions(boardState);
        if (misplacedPositions.size() == 0) {
            initValidatingState = InitValidatingState::VALIDATED;
            delegate->onBoardInitStateValid();
            GameStartedData data(side);
            delegate->onGameStarted(data);
            
            if (side == BaseTypes::Side::WHITE) {
                startPlayerTurn();
            } else {
                startOpponentTurn();
            }
        } else {
            delegate->onBoardInitStateInvalid(misplacedPositions);
        }
    }
}

//void OfflineGame::onPlayerChangedBoardState(const PlayerChangedBoardData& data) {
//    if (!isPlayerTurn) {
//        //TODO: handle INVALID ACTION
//    } else {
//        if (playerMovesFrom == "") {
////            if (data.at("state") == "on") {
////                //TODO: handle INVALID ACTION
////                return;
////            }
////
////            playerMovesFrom = data.at("square");
//        } else {
////            if (data.at("state") == "off") {
////                //TODO: handle INVALID ACTION
////                return;
////            }
////
////            std::string playerMovesTo = data.at("square");
////            std::string move = playerMovesFrom + playerMovesTo;
////            playerMovesFrom = "";
////            if (!validator->checkMove(move)) {
////                //TODO: handle INVALID MOVE
////                return;
////            }
//
////            validator->move(move);
////            engine->move(move);
////            moves.push_back(move);
////            EventData data;
////            delegate->onTurnEnded(data);
////            startOpponentTurn();
//        }
//    }
//}
