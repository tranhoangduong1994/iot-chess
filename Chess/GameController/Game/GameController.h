//
//  GameController.h
//  Chess
//
//  Created by macbook on 12/9/17.
//  Copyright © 2017 Tran Hoang Duong. All rights reserved.
//

#ifndef GameController_h
#define GameController_h

#include "BaseTypes.h"
#include "GameEventsProtocol.h"
#include "BoardServicesEventsProtocol.h"
#include "BoardKeyEventsProtocol.h"
#include "GameState.h"

#include "IEngine.h"
#include "IValidator.h"

#include <atomic>
#include <vector>

class GameController : public BoardServicesEventsProtocol, public BoardKeyEventsProtocol {
public:
    void start(BaseTypes::Side side, int difficulty);
    void setDelegate(GameEventsProtocol* delegate);
    
    void handleInitValidating(BaseTypes::Bitboard boardState);
    
    void handlePlayerFinishedMove(BaseTypes::Bitboard currentPhysicsBitboard);
    void handlePlayerTurnEnded(BaseTypes::Move move);

    void handleOpponentTurnEnded(BaseTypes::Move move);
    
    void handlePlayerRequestedGameMenu();
    void handlePlayerRequestedMoveValidating();
    
    void handleBackFromOffPositionScreen();
    
    //BoardServicsEventsProtocol implementation
    void onOpponentFinishedMove(BaseTypes::Move move, BaseTypes::Bitboard currentPhysicsBitboard) override;
    void onScanDone(BaseTypes::Bitboard currentPhysicsBitboard) override;
    void onBoardResetted(BaseTypes::Bitboard currentPhysicsBitboard) override;
    
    //BoardKeyEventsProtocol implementation
    void onKeyPressed(BoardKey key) override;
    
    GameController();
    ~GameController();
    
private:
    std::vector<BaseTypes::Move> readMove(BaseTypes::Bitboard currentPhysicsBitboard);
    void performPlayerTurn();
    void performOpponentTurn();
    
    GameEventsProtocol* delegate;
    
    IEngine* engine;
    IValidator* validator;
    
    BaseTypes::Side side;
    int difficulty;
    
    std::vector<BaseTypes::Move> moves;
    
    GameState* resettingState;
    GameState* initValidatingState;
    GameState* playerTurnState;
    GameState* opponentTurnState;
    GameState* currentState;
    
    BaseTypes::Bitboard currentLogicBitboard;
};

class ResettingState : public GameState {};

class InitValidatingState : public GameState {
public:
    void handleBoardScanningResult(GameController* game, BaseTypes::Bitboard currentPhysicsBitboard) override;
};

class PlayerTurnState : public GameState {
public:
    void handleKey(GameController* game, BoardKey key) override;
    void handleBoardScanningResult(GameController* game, BaseTypes::Bitboard currentPhysicsBitboard) override;
};

class OpponentTurnState : public GameState {};

#endif /* GameController_h */
