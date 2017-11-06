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

class GameEventsProtocol;

class OfflineGame : public BoardIngameEventsProtocol, public BoardKeyEventsProtocol {
public:
    void start(BaseTypes::Side side, int difficulty);
    void setDelegate(GameEventsProtocol* delegate);
    
    // BoardIngameEventsProtocol implementation
    void onMotorMoveDone() override;
    void onBoardStateChanged(const EventData& data) override;
    void onGameReset() override;
    
    // BoardKeyEventsProtocol implementation
    void onKeyPressed(const EventData& data) override;
    
private:
    void playerTurn();
    void computerTurn();
    
    GameEventsProtocol* delegate;
    
    IEngine* engine;
    IValidator* validator;
    
    int difficulty;
    
    std::vector<BaseTypes::Move> moves;
    bool isPlayerTurn;
    std::atomic<bool> hasComputerFinishedThinking;
    
    std::string playerMovesFrom;
};

#endif /* OfflineGame_h */
