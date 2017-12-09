//
//  GameController.cpp
//  Chess
//
//  Created by macbook on 12/9/17.
//  Copyright © 2017 Tran Hoang Duong. All rights reserved.
//

#include "GameController.h"

#include "StockfishEngine.h"
#include "PythonChessValidator.h"

#include "BoardServices.h"

#include "OptionScreen.h"

const BaseTypes::Bitboard BOARD_INIT_STATE("1111111111111111000000000000000000000000000000001111111111111111");

GameController::GameController() {
    engine = StockfishEngine::getInstance();
    validator = PythonChessValidator::getInstance();
    
    resettingState = new ResettingState();
    initValidatingState = new InitValidatingState();
    playerTurnState = new PlayerTurnState();
    opponentTurnState = new OpponentTurnState();
}

GameController::~GameController() {
    delete resettingState;
    delete initValidatingState;
    delete playerTurnState;
    delete opponentTurnState;
}

void GameController::start(BaseTypes::Side side, int difficulty) {
    this->side = side;
    this->difficulty = difficulty;
    
    moves.clear();
    
    currentState = resettingState;

    BoardServices::getInstance()->resetBoard();
}

void GameController::setDelegate(GameEventsProtocol* delegate) {
    this->delegate = delegate;
}

void GameController::handleInitValidating(BaseTypes::Bitboard boardState) {
    if (boardState != BOARD_INIT_STATE) {
        delegate->onPiecesOffPosition(boardState, BOARD_INIT_STATE);
        return;
    }
    
    currentLogicBitboard = BOARD_INIT_STATE;
    delegate->onGameStarted();
    
    if (side == BaseTypes::Side::WHITE) {
        performPlayerTurn();
        return;
    }
    
    performOpponentTurn();
}

void GameController::handlePlayerFinishedMove(BaseTypes::Bitboard currentPhysicsBitboard) {
    std::vector<BaseTypes::Move> availableMoves = game->readMove(currentPhysicsBitboard);
    if (availableMoves.size() < 1) {
        delegate->onInvalidMove();
    }
    
    if (availableMoves.size() == 1) {
        BaseTypes::Move move = availableMoves.at(0);
        currentLogicBitboard = currentPhysicsBitboard;
        handlePlayerTurnEnded(move);
        return;
    }
    
    delegate->onMultipleMovesAvailable(availableMoves, [=](bool moveSelected, BaseTypes::Move move) {
        if (moveSelected) {
            currentLogicBitboard = currentPhysicsBitboard;
            this->handlePlayerFinishedMove(move);
        }
    });
}

void GameController::handlePlayerTurnEnded(BaseTypes::Move move) {
    if (validator->checkDraw()) {
        delegate->onDrawGame(DrawGameType::INSUFFICENT_MATERIAL, move);
        return;
    }
    
    if (validator->checkGameOver()) {
        delegate->onWinGame(move);
        return;
    }
    
    performOpponentTurn();
}

void GameController::handleOpponentTurnEnded(BaseTypes::Move move) {
    if (validator->checkDraw()) {
        delegate->onDrawGame(DrawGameType::INSUFFICENT_MATERIAL, move);
        return;
    }
    
    if (validator->checkGameOver()) {
        delegate->onLoseGame(move);
        return;
    }
    
    performPlayerTurn();
}

void GameController::handlePlayerRequestedGameMenu() {
    std::vector<OptionScreenEntry> entries;
    
    OptionScreenEntry resetEntry;
    resetEntry.name = "Reset game";
    resetEntry.onSelected = [=](std::string content) {
        game->start(game->getSide(), game->getDifficulty());
    };
    entries.push_back(resetEntry);
    
    OptionScreenEntry cancelEntry;
    cancelEntry.name = "Cancel";
    cancelEntry.onSelected = [=](std::string content) {};
    entries.push_back(cancelEntry);
    
    OptionScreen* gameMenuScreen = OptionScreen::create("GAME MENU", entries);
    Screen::pushScreen(gameMenuScreen);
}

void GameController::handlePlayerRequestedMoveValidating() {
    BoardServices::getInstance()->scan();
}

void GameController::performPlayerTurn() {
    currentState = playerTurnState;
    if (moves.size() > 0) {
        delegate->onTurnBegan(moves.back());
        return;
    }
    delegate->onTurnBegan(BaseTypes::Move());
}

void GameController::performOpponentTurn() {
    currentState = opponentTurnState;
    engine->calculate([=](BaseTypes::Move move) {
        engine->move(move);
        validator->move(move);
        moves.push_back(move);
        BoardServices::getInstance()->move(move);
    });
}

std::vector<BaseTypes::Move> GameController::readMove(BaseTypes::Bitboard currentPhysicsBitboard) {
    std::vector<BaseTypes::Move> results;
    BaseTypes::Bitboard changedPositions = currentLogicBitboard ^ currentPhysicsBitboard;
    int popCount = changedPositions.popCount();
    
    if (popCount == 1) {//NORMAL CAPTURE
        BaseTypes::Bitboard fromBoard = currentLogicBitboard & changedPositions;
        int fromSquareIndex = fromBoard.getIndexOfSetBit(1);
        BaseTypes::Bitboard attackedPositionBoard(validator->getAttackedSquares(63 - fromSquareIndex));
        BaseTypes::Bitboard toBoard = currentLogicBitboard & attackedPositionBoard;
        int count = toBoard.popCount();
        
        for (int i = 1; i <= count; i++) {
            int toSquareIndex = toBoard.getIndexOfSetBit(i);
            BaseTypes::Position fromPos((char)(104 - fromSquareIndex % 8), 10 - fromSquareIndex / 8);
            BaseTypes::Position toPos((char)(104 - toSquareIndex % 8), 10 - toSquareIndex / 8);
            BaseTypes::Move move(fromPos, toPos);
            
            if (validator->checkMove(move)) {
                results.push_back(move);
            }
        }
        
        return results;
    }
    
    if (popCount == 2) {//NORMAL MOVE
        BaseTypes::Bitboard fromBoard = currentLogicBitboard & changedPositions;
        BaseTypes::Bitboard toBoard = currentPhysicsBitboard & changedPositions;
        int fromSquareIndex = fromBoard.getIndexOfSetBit(1);
        int toSquareIndex = toBoard.getIndexOfSetBit(1);
        
        BaseTypes::Position fromPos((char)(104 - fromSquareIndex % 8), 10 - fromSquareIndex / 8);
        BaseTypes::Position toPos((char)(104 - toSquareIndex % 8), 10 - toSquareIndex / 8);
        BaseTypes::Move move(fromPos, toPos);
        
        if (validator->checkMove(move)) {
            results.push_back(move);
        }
        
        return results;
    }
    
    if (popCount == 3) {//EN PASSANT CAPTURE
        BaseTypes::Bitboard changedPositionsShiftedDown = changedPositions << 8;
        BaseTypes::Bitboard changedPositionsShiftedUp = changedPositions >> 8;
        BaseTypes::Bitboard fromBoard = ((changedPositionsShiftedDown | changedPositionsShiftedUp) ^ changedPositions) & changedPositions;
        BaseTypes::Bitboard toBoard = currentPhysicsBitboard & changedPositions;
        int fromSquareIndex = fromBoard.getIndexOfSetBit(1);
        int toSquareIndex = toBoard.getIndexOfSetBit(1);
        
        BaseTypes::Position fromPos((char)(104 - fromSquareIndex % 8), 10 - fromSquareIndex / 8);
        BaseTypes::Position toPos((char)(104 - toSquareIndex % 8), 10 - toSquareIndex / 8);
        BaseTypes::Move move(fromPos, toPos);
        
        if (validator->checkMove(move)) {
            results.push_back(move);
        }
    }
    
    if (popCount == 4) {//CASTLING
        //TODO
        assert(false);
        return results;
    }
    
    return results;
}

void GameController::onOpponentFinishedMove(BaseTypes::Move move, BaseTypes::Bitboard currentPhysicsBitboard) {
    BaseTypes::Bitboard expectedBitboard = PythonChessValidator::getInstance()->getBitboard();
    if (expectedBitboard == currentPhysicsBitboard) {
        currentLogicBitboard = expectedBitboard;
        handleOpponentTurnEnded(move);
        return;
    }
    
    delegate->onPiecesOffPosition(currentPhysicsBitboard, expectedBitboard);
}

void GameController::onScanDone(BaseTypes::Bitboard currentPhysicsBitboard) {    
    currentState->handleBoardScanningResult(this, boardState);
}

void GameController::onKeyPressed(BoardKey key) {
    if (currentState) {
        currentState->handleKey(this, key);
    }
}

void GameController::onBoardResetted(BaseTypes::Bitboard currentPhysicsBitboard) {
    currentState = initValidatingState;
    currentState->handleBoardScanningResult(this, currentPhysicsBitboard);
    engine->start(difficulty);
    validator->start();
}

void InitValidatingState::handleBoardScanningResult(GameController *game, BaseTypes::Bitboard currentPhysicsBitboard) {
    game->handleInitValidating(currentPhysicsBitboard);
}

void PlayerTurnState::handleKey(GameController *game, BoardKey key)  {
    if (key == BoardKey::MENU) {
        game->handlePlayerRequestedGameMenu();
    }
    
    if (key == BoardKey::OK) {
        game->handlePlayerRequestedMoveValidating();
        return;
    }
}

void PlayerTurnState::handleBoardScanningResult(GameController *game, BaseTypes::Bitboard currentPhysicsBitboard) {
    
}
