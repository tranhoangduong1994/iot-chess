//
//  BoardServicesProtocol.h
//  Chess
//
//  Created by macbook on 11/4/17.
//  Copyright © 2017 Tran Hoang Duong. All rights reserved.
//

#ifndef BoardServicesProtocol_h
#define BoardServicesProtocol_h

#include "BaseTypes.h"

#include "BoardSystemEventsProtocol.h"
#include "BoardIngameEventsProtocol.h"
#include "BoardKeyEventsProtocol.h"

class BoardServicesProtocol {
public:
//    virtual void awaitStartingUp() = 0;
    virtual void resetGame() = 0;
    virtual void move(BaseTypes::Move move) = 0;
    virtual void capture(BaseTypes::Move move) = 0;
    virtual void scan() = 0;
    
    virtual void display(int line, std::string string) = 0;
    virtual void clearScreen() = 0;
    
    virtual void setBoardSystemEventsDelegate(BoardSystemEventsProtocol* s_delegate) = 0;
    virtual void setBoardIngameEventsDelegate(BoardIngameEventsProtocol* g_delegate) = 0;
    virtual void setBoardKeyEventsDelegate(BoardKeyEventsProtocol* k_delegate) = 0;
};

#endif /* BoardServicesProtocol_h */
