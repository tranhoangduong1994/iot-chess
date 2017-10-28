//
//  RoomEventsProtocol.h
//  Chess
//
//  Created by macbook on 10/22/17.
//  Copyright © 2017 Tran Hoang Duong. All rights reserved.
//

#ifndef RoomEventsProtocol_h
#define RoomEventsProtocol_h

#include "BaseTypes.h"

class RoomEventsProtocol {
public:
    virtual void onRoomCreatedSucceeded(const EventData& data) = 0;
    virtual void onRoomCreatedFailed(const EventData& data) = 0;
    
    virtual void onJoinedRoomSucceeded(const EventData& data) = 0;
    virtual void onJoinedRoomFailed(const EventData& data) = 0;
    
    virtual void onOpponentJoined(const EventData& data) = 0;
    virtual void onOpponentQuited(const EventData& data) = 0;
    virtual void onOpponentDisconnected(const EventData& data) = 0;
};

#endif /* RoomEventsProtocol_h */
