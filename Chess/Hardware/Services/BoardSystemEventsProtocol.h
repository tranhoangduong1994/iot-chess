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

struct SerialPortConnectedData {
    int file_description;
    SerialPortConnectedData(int fd) {
        file_description = fd;
    }
};

class BoardSystemEventsProtocol {
public:
    virtual void onSerialPortConnected(const SerialPortConnectedData& data) = 0;
    virtual void onSerialPortDisconnected() = 0;
};

#endif /* BoardSystemEventsProtocol_h */
