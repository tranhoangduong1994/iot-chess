//
//  OfflineGame.h
//  Chess
//
//  Created by macbook on 10/23/17.
//  Copyright © 2017 Tran Hoang Duong. All rights reserved.
//

#ifndef OfflineGame_h
#define OfflineGame_h

#define OFFLINE
#define ONLINE

#include "BaseTypes.h"
#include "IEngine.h"
#include "IValidator.h"
#include "BoardIngameEventsProtocol.h"
#include "BoardKeyEventsProtocol.h"
#include <atomic>
#include <vector>

enum GameState {
    NONE,
    INIT_STATE_VALIDATING,
    INIT_STATE_VALIDATED,
    MOVE_VALIDATING,
    MOVE_VALIDATED
};

class GameEventsProtocol;

class OfflineGame : public BoardIngameEventsProtocol, public BoardKeyEventsProtocol {
public:
    void start(BaseTypes::Side side, int difficulty);
    void setDelegate(GameEventsProtocol* delegate);
    
    // BoardIngameEventsProtocol implementation
    void onOpponentFinishedMove(const std::string& data) override;
    void onScanDone(const std::string& boardState) override;
    void onBoardStateChanged(const std::string& boardState) override;
    void onBoardResetted() override;
    
    // BoardKeyEventsProtocol implementation
    void onKeyPressed(const KeyPressedData& data) override;
    
    ~OfflineGame();
    
private:
    std::vector<BaseTypes::Position> getBoardStateMisplacedPositions(const std::string& boardState);
    
    void startPlayerTurn();
    void startOpponentTurn();
    
    std::vector<BaseTypes::Move> readMove(const BaseTypes::Bitboard& newState);
    
    void onPlayerFinishedMove(BaseTypes::Move move);
    
    GameEventsProtocol* delegate;
    
    IEngine* engine;
    IValidator* validator;
    
    BaseTypes::Side side;
    int difficulty;
    
    std::vector<BaseTypes::Move> moves;
    bool isPlayerTurn;
    std::atomic<bool> hasComputerFinishedThinking;
    
    std::string playerMovesFrom;
    
    GameState gameState;
    BaseTypes::Bitboard currentBitboard;
};

#endif /* OfflineGame_h */
