//
//  GameController.h
//  Chess
//
//  Created by macbook on 10/23/17.
//  Copyright © 2017 Tran Hoang Duong. All rights reserved.
//

#ifndef GameController_h
#define GameController_h

#define OFFLINE
#define ONLINE

#include "BaseTypes.h"
#include "GameEventsProtocol.h"
#include "NodeConnectorImplementation.h"

class GameController : public GameEventsProtocol {
public:
//    // Game events protocol implementation
//    void onGameStarted() override;
//
//    void onTurnBegan() override;
//    void onTurnEnded() override;
//    void onInvalidMove() override;
//
//    void onWinGame() override;
//    void onLoseGame() override;
//    void onDrawGame() override;
    
    void move(BaseTypes::Move move);
    
    OFFLINE void play(BaseTypes::Side side, int difficulty);
    
    
    
    ONLINE void onDrawGameOffered() override;
    ONLINE void onOpponentSurrendered() override;
    ONLINE offerDrawGame();
    ONLINE surrender();
    
};

#endif /* GameController_h */
