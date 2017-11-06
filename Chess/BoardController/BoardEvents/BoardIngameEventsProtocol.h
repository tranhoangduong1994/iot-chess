//
//  BoardIngameEventsProtocol.h
//  Chess
//
//  Created by macbook on 11/5/17.
//  Copyright © 2017 Tran Hoang Duong. All rights reserved.
//

#ifndef BoardIngameEventsProtocol_h
#define BoardIngameEventsProtocol_h

class BoardIngameEventsProtocol {
public:
    virtual void onMoveDone() = 0;
    virtual void onScanDone(EventData data) = 0;
    virtual void onGameReset() = 0;
};

#endif /* BoardIngameEventsProtocol_h */
