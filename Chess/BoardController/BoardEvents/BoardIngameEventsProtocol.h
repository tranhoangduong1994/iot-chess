//
//  BoardIngameEventsProtocol.h
//  Chess
//
//  Created by macbook on 11/5/17.
//  Copyright © 2017 Tran Hoang Duong. All rights reserved.
//

#ifndef BoardIngameEventsProtocol_h
#define BoardIngameEventsProtocol_h

#include "BaseTypes.h"

class BoardIngameEventsProtocol {
public:
    virtual void onPlayerFinishedMove(const std::string& data) = 0;
    virtual void onOpponentFinishedMove(const std::string& data) = 0;
    virtual void onScanDone(const std::string& boardState) = 0;
    virtual void onBoardResetted() = 0;
};

#endif /* BoardIngameEventsProtocol_h */
