//
//  ConnectingEventsProtocol.h
//  Chess
//
//  Created by macbook on 10/22/17.
//  Copyright © 2017 Tran Hoang Duong. All rights reserved.
//

#ifndef ConnectingEventsProtocol_h
#define ConnectingEventsProtocol_h

class ConnectingEventsProtocol {
public:
    virtual void onConnected(const EventData& data) = 0;
    virtual void onLoggedInSucceeded(const EventData& data) = 0;
    virtual void onLoggedInFailed(const EventData& data) = 0;
};

#endif /* ConnectingEventsProtocol_h */
