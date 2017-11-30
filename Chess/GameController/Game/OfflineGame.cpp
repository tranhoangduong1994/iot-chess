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

//const std::string BOARD[] = {
//    "A1", "A2", "A3", "A4", "A5", "A6", "A7", "A8",
//    "B1", "B2", "B3", "B4", "B5", "B6", "B7", "B8",
//    "C1", "C2", "C3", "C4", "C5", "C6", "C7", "C8",
//    "D1", "D2", "D3", "D4", "D5", "D6", "D7", "D8",
//    "E1", "E2", "E3", "E4", "E5", "E6", "E7", "E8",
//    "F1", "F2", "F3", "F4", "F5", "F6", "F7", "F8",
//    "G1", "G2", "G3", "G4", "G5", "G6", "G7", "G8",
//    "H1", "H2", "H3", "H4", "H5", "H6", "H7", "H8",
//};

const BaseTypes::Bitboard BOARD_INIT_STATE("1111111111111111000000000000000000000000000000001111111111111111");

OfflineGame::~OfflineGame() {    
    BoardServices::getInstance()->setBoardIngameEventsDelegate(NULL);
    BoardServices::getInstance()->setBoardKeyEventsDelegates(NULL);
}

void OfflineGame::start(BaseTypes::Side side, int difficulty) {
    engine = StockfishEngine::getInstance();
    validator = PythonChessValidator::getInstance();
    
    BoardServices::getInstance()->setBoardIngameEventsDelegate(this);
    BoardServices::getInstance()->setBoardKeyEventsDelegate(this);
    
    this->side = side;
    this->difficulty = difficulty;
    
    engine->start(difficulty);
    validator->start();
    
    gameState = GameState::INIT_STATE_VALIDATING;
    BoardServices::getInstance()->scan();
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

void OfflineGame::onPlayerFinishedMove(BaseTypes::Move move) {
    if (validator->checkDraw()) {
        delegate->onDrawGame(DrawGameData(move.toString()));
        return;
    }
    if (validator->checkGameOver()) {
        delegate->onWinGame(WinGameData(move.toString()));
        return;
    }
    startOpponentTurn();
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

void OfflineGame::onBoardStateChanged(const std::string& boardState) {
    onScanDone(boardState);
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
    if (data.key == BoardKey::OK) {
        if (!isPlayerTurn) return;
        if (gameState == GameState::NONE || gameState == GameState::INIT_STATE_VALIDATING) {
            gameState = GameState::INIT_STATE_VALIDATING;
            BoardServices::getInstance()->scan();
        }
        
        if (gameState == GameState::MOVE_VALIDATING || gameState == GameState::MOVE_VALIDATED) {
            gameState = GameState::MOVE_VALIDATING;
            BoardServices::getInstance()->scan();
        }
    } else if (data.key == BoardKey::MENU) {
        
    }
}

void OfflineGame::onScanDone(const std::string& boardState) {
    BaseTypes::Bitboard boardStateByBit(boardState);
    if (gameState == GameState::INIT_STATE_VALIDATING) {
        
        if (boardStateByBit != BOARD_INIT_STATE) {
            delegate->onBoardInitStateInvalid(BOARD_INIT_STATE.getMisplacedPositions(boardState));
            return;
        }
        
        gameState = GameState::INIT_STATE_VALIDATED;
        currentBitboard = BOARD_INIT_STATE;
        delegate->onBoardInitStateValid();
        GameStartedData data(side);
        delegate->onGameStarted(data);
        
        if (side == BaseTypes::Side::WHITE) {
            startPlayerTurn();
        } else {
            startOpponentTurn();
        }
        
        return;
    }
    
    if (gameState == GameState::MOVE_VALIDATING) {
        std::vector<BaseTypes::Move> moves = readMove(boardState);
        if (moves.size() < 1) {
            InvalidMoveData data("");
            delegate->onInvalidMove(data);
            return;
        }
        
        gameState = GameState::MOVE_VALIDATED;
        
        if (moves.size() == 1) {
            BaseTypes::Move move = moves.at(0);
            engine->move(move);
            validator->move(move);
            this->onPlayerFinishedMove(move);
            return;
        }
        
        delegate->onMultipleMovesAvailable(moves, [=](bool moveSelected, BaseTypes::Move move) {
            if (moveSelected) {
                engine->move(move);
                validator->move(move);
                this->onPlayerFinishedMove(move);
            }
        });
    }
}

std::vector<BaseTypes::Move> OfflineGame::readMove(const BaseTypes::Bitboard& newState) {
    std::vector<BaseTypes::Move> result;
    BaseTypes::Bitboard changedPositions = currentBitboard ^ newState;
    int popCount = changedPositions.popCount();
    
    if (popCount == 1) {//NORMAL CAPTURE
        BaseTypes::Bitboard fromBoard = currentBitboard & changedPositions;
        int fromSquareIdx = fromBoard.getIndexOfSetBit(1);
        
        BaseTypes::Bitboard attackedPositionBoard(validator->getAttackedSquares(fromSquareIdx));
        
        BaseTypes::Bitboard toBoard = currentBitboard & attackedPositionBoard;
        int count = toBoard.popCount();
        
        for (int i = 0; i < count; i++) {
            int toSquareIdx = toBoard.getIndexOfSetBit(i);
            std::string moveString = "";
            moveString += (char)((fromSquareIdx % 8) + 97);
            moveString += (char)((fromSquareIdx / 8) + 1);
            moveString += (char)((toSquareIdx % 8) + 97);
            moveString += (char)((toSquareIdx / 8) + 1);
            
            BaseTypes::Move move(moveString);
            if (validator->checkMove(move)) {
                result.push_back(move);
            }
        }
        
        return result;
    }
    
    if (popCount == 2) {//NORMAL MOVE
        BaseTypes::Bitboard fromBoard = currentBitboard & changedPositions;
        BaseTypes::Bitboard toBoard = newState & changedPositions;
        int fromSquareIdx = fromBoard.getIndexOfSetBit(1);
        int toSquareIdx = toBoard.getIndexOfSetBit(1);
        
        std::string moveString = "";
        moveString += (char)((fromSquareIdx % 8) + 97);
        moveString += (char)((fromSquareIdx / 8) + 1);
        moveString += (char)((toSquareIdx % 8) + 97);
        moveString += (char)((toSquareIdx / 8) + 1);
        
        
        
        BaseTypes::Move move(moveString);
        if (validator->checkMove(move)) {
            result.push_back(move);
        }
        
        return result;
    }
    
    if (popCount == 3) {//EN PASSANT CAPTURE
        BaseTypes::Bitboard fromBoard = ((changedPositions << 8) | (changedPositions >> 8)) ^ changedPositions & changedPositions;
        BaseTypes::Bitboard toBoard = newState & changedPositions;
        int fromSquareIdx = fromBoard.getIndexOfSetBit(1);
        int toSquareIdx = toBoard.getIndexOfSetBit(1);

        std::string moveString = "";
        moveString += (char)((fromSquareIdx % 8) + 97);
        moveString += (char)((fromSquareIdx / 8) + 1);
        moveString += (char)((toSquareIdx % 8) + 97);
        moveString += (char)((toSquareIdx / 8) + 1);
        
        BaseTypes::Move move(moveString);
        if (validator->checkMove(move)) {
            result.push_back(move);
        }
        
        return result;
    }
    
    if (popCount == 4) {//CASTLING
        return result;
    }
    
    return result;
}
