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

#include "OptionScreen.h"

#include "thread.h"

const BaseTypes::Bitboard BOARD_INIT_STATE("1111111111111111000000000000000000000000000000001111111111111111");

OfflineGame::~OfflineGame() {    
    BoardServices::getInstance()->setBoardIngameEventsDelegate(NULL);
    BoardServices::getInstance()->setBoardKeyEventsDelegate(NULL);
}

void OfflineGame::start(BaseTypes::Side side, int difficulty) {
    engine = StockfishEngine::getInstance();
    validator = PythonChessValidator::getInstance();
    
    this->side = side;
    this->difficulty = difficulty;
    
    moves.clear();
    
    BoardServices::getInstance()->resetBoard();
}

void OfflineGame::startPlayerTurn() {
    isPlayerTurn = true;
    gameState = GameState::MOVE_VALIDATING;
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

void OfflineGame::onOpponentFinishedMove(const std::string& data, const std::string& newBoardState) {
    if (validator->checkDraw()) {
        delegate->onDrawGame(data);
        return;
    }
    if (validator->checkGameOver()) {
        delegate->onLoseGame(data);
        return;
    }
    
    std::cout << "[OfflineGame] onOpponentFinishedMove: " << newBoardState << std::endl;
    currentBitboard = newBoardState;
    
    startPlayerTurn();
}

void OfflineGame::onBoardResetted() {
    engine->start(difficulty);
    validator->start();
    
    gameState = GameState::INIT_STATE_VALIDATING;
    BoardServices::getInstance()->scan();
}

void OfflineGame::onKeyPressed(const KeyPressedData& data) {
    if (data.key == BoardKey::OK) {
        if (gameState == GameState::NONE || gameState == GameState::INIT_STATE_VALIDATING) {
            gameState = GameState::INIT_STATE_VALIDATING;
            BoardServices::getInstance()->scan();
        }
        
        if (gameState == GameState::MOVE_VALIDATING || gameState == GameState::MOVE_VALIDATED) {
			if (!isPlayerTurn) return;
            gameState = GameState::MOVE_VALIDATING;
            BoardServices::getInstance()->scan();
        }
    } else if (data.key == BoardKey::MENU) {
		if (!isPlayerTurn) return;
		
		std::vector<OptionScreenEntry> entries;
		
		OptionScreenEntry resetEntry;
		resetEntry.name = "Reset game";
		resetEntry.onSelected = [=](std::string content) {
			start(side, difficulty);
		};
		entries.push_back(resetEntry);
		
		OptionScreenEntry cancelEntry;
		cancelEntry.name = "Cancel";
		cancelEntry.onSelected = [=](std::string content) {};
		entries.push_back(cancelEntry);
		
		OptionScreen* gameMenuScreen = OptionScreen::create("GAME MENU", entries);
		Screen::pushScreen(gameMenuScreen);
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
        std::vector<BaseTypes::Move> availableMoves = readMove(boardState);
        if (availableMoves.size() < 1) {
            InvalidMoveData data("");
            delegate->onInvalidMove(data);
            return;
        }
        
        gameState = GameState::MOVE_VALIDATED;
        
        if (availableMoves.size() == 1) {
            BaseTypes::Move move = availableMoves.at(0);
            engine->move(move);
            validator->move(move);
            this->onPlayerFinishedMove(move);
            return;
        }
        
        delegate->onMultipleMovesAvailable(availableMoves, [=](bool moveSelected, BaseTypes::Move move) {
            if (moveSelected) {
                engine->move(move);
                validator->move(move);
                this->onPlayerFinishedMove(move);
            }
        });
    }
}

std::vector<BaseTypes::Move> OfflineGame::readMove(const BaseTypes::Bitboard& newState) {
	std::cout << "[OfflineGame] readMove.";
    std::vector<BaseTypes::Move> result;
    BaseTypes::Bitboard changedPositions = currentBitboard ^ newState;
    int popCount = changedPositions.popCount();
    std::cout << " Changed positions = " << changedPositions.toString() << ",";
    std::cout << " Pop count result = " << std::to_string(popCount) << ".";
    
    if (popCount == 1) {//NORMAL CAPTURE
		std::cout << " Normal capture.";
        BaseTypes::Bitboard fromBoard = currentBitboard & changedPositions;
        int fromSquareIdx = fromBoard.getIndexOfSetBit(1);
        std::cout << " Move from: " << fromSquareIdx << ".";        
        BaseTypes::Bitboard attackedPositionBoard(validator->getAttackedSquares(63 - fromSquareIdx));
		std::cout << " attackedPositionBoard: " << attackedPositionBoard.toString() << ".";
        BaseTypes::Bitboard toBoard = currentBitboard & attackedPositionBoard;
        std::cout << " toBoard: " << toBoard.toString() << "." << std::endl;
        int count = toBoard.popCount();
        
        for (int i = 1; i <= count; i++) {
            int toSquareIdx = toBoard.getIndexOfSetBit(i);
            std::cout << "To square idx = " << toSquareIdx << std::endl;
            std::string moveString = "";
			moveString += (char)(7 - (fromSquareIdx % 8) + 97);
			moveString += std::to_string(9 - ((fromSquareIdx / 8) + 1));
			moveString += (char)(7 - (toSquareIdx % 8) + 97);
			moveString += std::to_string(9 - ((toSquareIdx / 8) + 1));
            
            BaseTypes::Move move(moveString);
            if (validator->checkMove(move)) {
                result.push_back(move);
            }
        }
        
        return result;
    }
    
    if (popCount == 2) {//NORMAL MOVE
		std::cout << " Normal move.";
        BaseTypes::Bitboard fromBoard = currentBitboard & changedPositions;
        BaseTypes::Bitboard toBoard = newState & changedPositions;
        int fromSquareIdx = fromBoard.getIndexOfSetBit(1);
        int toSquareIdx = toBoard.getIndexOfSetBit(1);
        
        std::cout << " From square board = " << fromBoard.toString() << ". To square board = " << toBoard.toString() << ".";
        std::cout << " From square idx = " << fromSquareIdx << ". To square idx = " << toSquareIdx;
        
        std::string moveString = "";
        moveString += (char)(7 - (fromSquareIdx % 8) + 97);
        moveString += std::to_string(9 - ((fromSquareIdx / 8) + 1));
        moveString += (char)(7 - (toSquareIdx % 8) + 97);
        moveString += std::to_string(9 - ((toSquareIdx / 8) + 1));
        
        std::cout << " " << moveString << std::endl;
        
        BaseTypes::Move move(moveString);
        if (validator->checkMove(move)) {
			std::cout << "[OfflineGame] move: " << moveString << " is valid" << std::endl;
            result.push_back(move);
        } else {
			std::cout << "[OfflineGame] move: " << moveString << " is NOT valid" << std::endl;
		}
        
        return result;
    }
    
    if (popCount == 3) {//EN PASSANT CAPTURE
		std::cout << " En passant capture." << std::endl;
        BaseTypes::Bitboard changedPositionShiftedDown = changedPositions << 8;
        BaseTypes::Bitboard changedPositionShiftedUp = changedPositions >> 8;
        BaseTypes::Bitboard fromBoard = ((changedPositionShiftedDown | changedPositionShiftedUp) ^ changedPositions) & changedPositions;
        BaseTypes::Bitboard toBoard = newState & changedPositions;
        int fromSquareIdx = fromBoard.getIndexOfSetBit(1);
        int toSquareIdx = toBoard.getIndexOfSetBit(1);

        std::string moveString = "";
        moveString += (char)(7 - (fromSquareIdx % 8) + 97);
        moveString += std::to_string(9 - ((fromSquareIdx / 8) + 1));
        moveString += (char)(7 - (toSquareIdx % 8) + 97);
        moveString += std::to_string(9 - ((toSquareIdx / 8) + 1));
        
        BaseTypes::Move move(moveString);
        if (validator->checkMove(move)) {
            result.push_back(move);
        }
        
        return result;
    }
    
    if (popCount == 4) {//CASTLING
		std::cout << " Castling." << std::endl;
        return result;
    }
    
    std::cout << " Move is not valid," << std::endl; 
    
    return result;
}
