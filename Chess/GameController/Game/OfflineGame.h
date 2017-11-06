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
#include <atomic>
#include <vector>

class GameEventsProtocol;

class OfflineGame {
private:
    GameEventsProtocol* delegate;
    
    IEngine* engine;
    IValidator* validator;
    
    std::vector<BaseTypes::Move> moves;
    bool isPlayerTurn;
    std::atomic<bool> hasComputerFinishedThinking;
    
    void playerTurn();
    void computerTurn();
    
public:
    void start(BaseTypes::Side side, int difficulty);
    void move(BaseTypes::Move move);
    void setDelegate(GameEventsProtocol* delegate);
};

#endif /* OfflineGame_h */
