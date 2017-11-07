//
//  BoardSystemEventsProtocol.h
//  Chess
//
//  Created by macbook on 11/5/17.
//  Copyright © 2017 Tran Hoang Duong. All rights reserved.
//

#ifndef BoardSystemEventsProtocol_h
#define BoardSystemEventsProtocol_h

#include "BaseTypes.h"

class BoardSystemEventsProtocol {
public:
    virtual void onSerialPortConnected(const EventData& data) = 0;
};

#endif /* BoardSystemEventsProtocol_h */