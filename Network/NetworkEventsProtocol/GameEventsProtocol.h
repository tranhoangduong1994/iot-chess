//
//  GameEventsProtocol.h
//  Chess
//
//  Created by macbook on 10/22/17.
//  Copyright © 2017 Tran Hoang Duong. All rights reserved.
//

#ifndef GameEventsProtocol_h
#define GameEventsProtocol_h

#include "BaseTypes.h"

class GameEventsProtocol {
public:
    virtual void onGameStarted(const EventData& data) = 0;
    
    virtual void onTurnBegan(const EventData& data) = 0;
    virtual void onTurnEnded(const EventData& data) = 0;
    virtual void onInvalidMove(const EventData& data) = 0;
    
    virtual void onDrawGameOffered(const EventData& data) = 0;
    virtual void onOpponentSurrendered(const EventData& data) = 0;
    
    virtual void onWinGame(const EventData& data) = 0;
    virtual void onLoseGame(const EventData& data) = 0;
    virtual void onDrawGame(const EventData& data) = 0;
};

#endif /* GameEventsProtocol_h */
