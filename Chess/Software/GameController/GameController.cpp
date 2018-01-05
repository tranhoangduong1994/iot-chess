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

const BaseTypes::Bitboard GAME_INIT_BITBOARD("1111111111111111000000000000000000000000000000001111111111111111");

const BaseTypes::Bitboard CASTLING_BITBOARD("1011111100000000000000000000000000000000000000000000000010111111");
BaseTypes::Bitboard WHITE_QUEEN_SIDE_CASTLING_BITBOARD("1011100000000000000000000000000000000000000000000000000000000000");
BaseTypes::Bitboard WHITE_KING_SIDE_CASTLING_BITBOARD ("0000111100000000000000000000000000000000000000000000000000000000");
BaseTypes::Bitboard BLACK_QUEEN_SIDE_CASTLING_BITBOARD("0000000000000000000000000000000000000000000000000000000010111000");
BaseTypes::Bitboard BLACK_KING_SIDE_CASTLING_BITBOARD ("0000000000000000000000000000000000000000000000000000000000001111");

GameController::GameController(BaseTypes::Side side, int difficulty) {
    this->side = side;
    this->difficulty = difficulty;
    
    engine = StockfishEngine::getInstance();
    validator = PythonChessValidator::getInstance();
    
    resettingState = new ResettingState();
    multipleMovesSelectingState = new MultipleMoveSelectingState();
    playerTurnState = new PlayerTurnState();
    opponentTurnState = new OpponentTurnState();
}

GameController::~GameController() {
    delete resettingState;
    delete multipleMovesSelectingState;
    delete playerTurnState;
    delete opponentTurnState;
}

void GameController::start() {
	std::cout << "[GameController] start" << std::endl;
    
    moves.clear();
    
    currentState = resettingState;

    BoardServices::getInstance()->resetBoard();
    delegator->onResetting();
}

void GameController::setDelegator(GameEventsProtocol* delegator) {
    this->delegator = delegator;
}

void GameController::setSide(BaseTypes::Side side) {
    this->side = side;
}

void GameController::setDifficulty(int difficulty) {
    this->difficulty = difficulty;
}

void GameController::handleInitValidating(BaseTypes::Bitboard boardState) {
    std::cout << "[GameController] handleInitValidating" << std::endl;
    if (boardState != GAME_INIT_BITBOARD) {
        BoardServices::getInstance()->playSound(SoundType::SOUND_OFFPOSITION);
        delegator->onPiecesOffPosition(boardState, GAME_INIT_BITBOARD);
        return;
    }
    
    BoardServices::getInstance()->playSound(SoundType::SOUND_GAME_READY);
    
    currentLogicBitboard = GAME_INIT_BITBOARD;
    delegator->onGameStarted();
    
    if (side == BaseTypes::Side::WHITE) {
        performPlayerTurn();
        return;
    }
    
    performOpponentTurn();
}

void GameController::handlePlayerFinishedMove(BaseTypes::Bitboard currentPhysicsBitboard) {
    std::vector<BaseTypes::Move> availableMoves = readMove(currentPhysicsBitboard);
    std::cout << "[GameController] handlePlayerFinishedMove, available moves = " << std::to_string(availableMoves.size()) << std::endl;
    if (availableMoves.size() < 1) {
        BoardServices::getInstance()->playSound(SoundType::SOUND_INVALID_MOVE);
        delegator->onInvalidMove();
        return;
    }
    
    if (availableMoves.size() == 1) {
        BoardServices::getInstance()->playSound(SoundType::SOUND_VALID_MOVE);
        BaseTypes::Move move = availableMoves.at(0);

        currentLogicBitboard = currentPhysicsBitboard;
        
        if (tryPromotion(move)) {
            return;
        }
        
        handlePlayerTurnEnded(move);
        return;
    }
    
    BoardServices::getInstance()->playSound(SoundType::SOUND_MULTIPLEMOVE);
    currentState = multipleMovesSelectingState;
    delegator->onMultipleMovesAvailable(availableMoves, [=](bool moveSelected, BaseTypes::Move move) {
        if (moveSelected) {
            BoardServices::getInstance()->playSound(SoundType::SOUND_VALID_MOVE);
            currentLogicBitboard = currentPhysicsBitboard;
            
            if (tryPromotion(move)) {
                return;
            }
            
            this->handlePlayerTurnEnded(move);
            return;
        }
        currentState = playerTurnState;
    });
}

bool GameController::tryPromotion(BaseTypes::Move move_) {
    BaseTypes::Move move(move_);
    BaseTypes::Position fromPos = move.fromPos;
    BaseTypes::Position toPos = move.toPos;
    BaseTypes::PieceType pieceType = validator->getPieceType(fromPos.toSquareIndex());
    
    if (pieceType == BaseTypes::PieceType::WHITE_PAWN && toPos.rank == 8 || pieceType == BaseTypes::PieceType::BLACK_PAWN && toPos.rank == 1) {
        delegator->onPlayerPromotion([=](std::string pieceType) {
            if (pieceType == "Queen") {
                move.promotionType = 'q';
                handlePlayerTurnEnded(move);
                return;
            }
            
            if (pieceType == "Rook") {
                move.promotionType = 'r';
                handlePlayerTurnEnded(move);
                return;
            }
            
            if (pieceType == "Bishop") {
                move.promotionType = 'b';
                handlePlayerTurnEnded(move);
                return;
            }
            
            if (pieceType == "Knight") {
                move.promotionType = 'n';
                handlePlayerTurnEnded(move);
                return;
            }
        });
        return true;
    }
    
    return false;
}

void GameController::handlePlayerTurnEnded(BaseTypes::Move move) {
    BaseTypes::PieceType pieceType = validator->getPieceType(move.fromPos.toSquareIndex());
    move.pieceType = pieceType;
	moves.push_back(move);
	engine->move(move);
	validator->move(move);
	
    if (validator->checkDraw()) {
        delegator->onDrawGame(DrawGameType::INSUFFICENT_MATERIAL, move);
        return;
    }
    
    if (validator->checkGameOver()) {
        delegator->onWinGame(move);
        return;
    }
    
    delegator->onTurnEnded(move);
    
    performOpponentTurn();
}

void GameController::handleOpponentTurnEnded(BaseTypes::Move move) {
    if (validator->checkDraw()) {
        delegator->onDrawGame(DrawGameType::INSUFFICENT_MATERIAL, move);
        return;
    }
    
    if (validator->checkGameOver()) {
        delegator->onLoseGame(move);
        return;
    }
    
    performPlayerTurn();
}

void GameController::handlePlayerRequestedGameMenu() {
    delegator->onGameMenuRequested();
}

void GameController::handlePlayerRequestedMoveValidating() {
    BoardServices::getInstance()->scan();
}

void GameController::handleBackFromOffPositionScreen() {
    std::cout << "[GameController] handleBackFromOffPositionScreen" << std::endl;
	if (moves.size() > 0) {
		handleOpponentTurnEnded(moves.back());
		return;
	}
	
	handleInitValidating(GAME_INIT_BITBOARD);
}

void GameController::performPlayerTurn() {
    currentState = playerTurnState;
    if (moves.size() > 0) {
        delegator->onTurnBegan(moves.back());
        return;
    }
    delegator->onTurnBegan(BaseTypes::Move());
}

void GameController::performOpponentTurn() {
    std::cout << "[GameController] performOpponentTurn" << std::endl;
    currentState = opponentTurnState;
    engine->calculate([=](BaseTypes::Move move) {
        handleComputerFinishedThinking(move);
    });
}

void GameController::handleComputerFinishedThinking(BaseTypes::Move move) {
    BaseTypes::Position fromPos = move.fromPos;
    BaseTypes::Position toPos = move.toPos;
    BaseTypes::PieceType pieceType = validator->getPieceType(fromPos.toSquareIndex());
    move.pieceType = pieceType;
    engine->move(move);
    validator->move(move);
    moves.push_back(move);
    if (pieceType == BaseTypes::PieceType::WHITE_KING) {
        if (toPos.file - fromPos.file == 2) {
            BoardServices::getInstance()->castling(CastlingType::WHITE_KING_SIDE);
            return;
        }
        if (toPos.file - fromPos.file == -2) {
            BoardServices::getInstance()->castling(CastlingType::WHITE_QUEEN_SIDE);
            return;
        }
    }
    
    if (pieceType == BaseTypes::PieceType::BLACK_KING) {
        if (toPos.file - fromPos.file == 2) {
            BoardServices::getInstance()->castling(CastlingType::BLACK_KING_SIDE);
            return;
        }
        if (toPos.file - fromPos.file == -2) {
            BoardServices::getInstance()->castling(CastlingType::BLACK_QUEEN_SIDE);
            return;
        }
    }
    
    if (pieceType == BaseTypes::PieceType::WHITE_PAWN || pieceType == BaseTypes::PieceType::BLACK_PAWN) {
        if (move.toPos.file != move.fromPos.file) {//THE PAWN CAPTURES A PIECE
            int toPosIndex = (move.toPos.rank - 1) * 8 + (move.toPos.file - 'a');
            bool toPosState = currentLogicBitboard.get(toPosIndex);
            if (!toPosState) {//EN PASSANT
                BoardServices::getInstance()->enPassant(move);
                return;
            }
        }
    }
    BoardServices::getInstance()->move(move);
}

void GameController::onOpponentFinishedMove(BaseTypes::Move move, BaseTypes::Bitboard currentPhysicsBitboard) {
	std::cout << "[GameController] onOpponentFinishedMove" << std::endl;
	std::cout << "currentLogicBitboard: " << currentLogicBitboard.toString() << std::endl;
	std::cout << "currentPhysicsBitboard: " << currentPhysicsBitboard.toString() << std::endl;
    BaseTypes::Bitboard expectedBitboard = PythonChessValidator::getInstance()->getBitboard();
    std::cout << "expectedBitboard: " << expectedBitboard.toString() << std::endl;
    currentLogicBitboard = expectedBitboard;
    if (expectedBitboard == currentPhysicsBitboard) {
        handleOpponentTurnEnded(move);
        return;
    }
    
    BoardServices::getInstance()->playSound(SoundType::SOUND_OFFPOSITION);
    delegator->onPiecesOffPosition(currentPhysicsBitboard, expectedBitboard);
}

void GameController::onScanDone(BaseTypes::Bitboard currentPhysicsBitboard) {    
    currentState->handleBoardScanningResult(this, currentPhysicsBitboard);
}

void GameController::onKeyPressed(BoardKey key) {
    currentState->handleKey(this, key);
}

void GameController::onBoardResetted(BaseTypes::Bitboard currentPhysicsBitboard) {
	std::cout << "[GameController] onBoardResetted" << std::endl;
    engine->start(difficulty);
    validator->start();
    handleInitValidating(currentPhysicsBitboard);
}

std::vector<BaseTypes::Move> GameController::readMove(BaseTypes::Bitboard currentPhysicsBitboard) {
	std::cout << "[GameController] readMove" << std::endl;
	std::cout << "currentLogicBitboard: " << currentLogicBitboard.toString() << std::endl;
	std::cout << "currentPhysicsBitboard: " << currentPhysicsBitboard.toString() << std::endl;
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
            BaseTypes::Position fromPos((char)(104 - fromSquareIndex % 8), 8 - fromSquareIndex / 8);
            BaseTypes::Position toPos((char)(104 - toSquareIndex % 8), 8 - toSquareIndex / 8);
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
        
        BaseTypes::Position fromPos((char)(104 - fromSquareIndex % 8), 8 - fromSquareIndex / 8);
        BaseTypes::Position toPos((char)(104 - toSquareIndex % 8), 8 - toSquareIndex / 8);
        BaseTypes::Move move(fromPos, toPos);
        
        std::cout << "From board: " << fromBoard.toString() << std::endl;
        std::cout << "To board: " << toBoard.toString() << std::endl;
        std::cout << "From squareIndex: " << fromSquareIndex << std::endl;
        std::cout << "To squareIndex: " << toSquareIndex << std::endl;
        std::cout << "From pos: " << fromPos.toString() << std::endl;
        std::cout << "To pos: " << toPos.toString() << std::endl;
        std::cout << "Normal move: " << move.toString() << std::endl;
        
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
        
        BaseTypes::Position fromPos((char)(104 - fromSquareIndex % 8), 8 - fromSquareIndex / 8);
        BaseTypes::Position toPos((char)(104 - toSquareIndex % 8), 8 - toSquareIndex / 8);
        BaseTypes::Move move(fromPos, toPos);
        
        if (validator->checkMove(move)) {
            results.push_back(move);
        }
        
        return results;
    }
    
    if (popCount == 4) {//CASTLING
        if (side == BaseTypes::Side::WHITE) {
            if ((changedPositions & CASTLING_BITBOARD) == WHITE_QUEEN_SIDE_CASTLING_BITBOARD) {
                results.push_back(BaseTypes::Move("e1c1"));
                return results;
            }
            
            if ((changedPositions & CASTLING_BITBOARD) == WHITE_KING_SIDE_CASTLING_BITBOARD) {
                results.push_back(BaseTypes::Move("e1g1"));
                return results;
            }
        }
        
        
        if ((changedPositions & CASTLING_BITBOARD) == BLACK_QUEEN_SIDE_CASTLING_BITBOARD) {
            results.push_back(BaseTypes::Move("e8c8"));
            return results;
        }
        
        if ((changedPositions & CASTLING_BITBOARD) == BLACK_KING_SIDE_CASTLING_BITBOARD) {
            results.push_back(BaseTypes::Move("e8g8"));
            return results;
        }
    }
    
    return results;
}

void PlayerTurnState::handleKey(GameController *gameController, BoardKey key)  {
    if (key == BoardKey::MENU) {
        gameController->handlePlayerRequestedGameMenu();
    }
    
    if (key == BoardKey::OK) {
        gameController->handlePlayerRequestedMoveValidating();
        return;
    }
}

void PlayerTurnState::handleBoardScanningResult(GameController *gameController, BaseTypes::Bitboard currentPhysicsBitboard) {
    gameController->handlePlayerFinishedMove(currentPhysicsBitboard);
}


