//
//  BaseConnectorProtocol.h
//  Chess
//
//  Created by macbook on 10/22/17.
//  Copyright © 2017 Tran Hoang Duong. All rights reserved.
//

#ifndef BaseConnectorProtocol_h
#define BaseConnectorProtocol_h

#include "BaseTypes.h"

class ConnectingEventsProtocol;
class RoomEventsProtocol;
class GameEventsProtocol;

class BaseConnectorProtocol {
public:
    virtual void setConnectingEventsDelegate(ConnectingEventsProtocol* delegate) = 0;
    virtual void setRoomEventsDelegate(RoomEventsProtocol* delegate) = 0;
    virtual void setGameEventsDelegate(GameEventsProtocol* delegate) = 0;
    
    //connecting functions
    virtual void connect(std::string host) = 0;
    virtual void login(std::string username, std::string password) = 0;
    
    //room functions
    virtual void createRoom(std::string roomName, BaseTypes::Side side) = 0;
    virtual void joinRoom(std::string roomId) = 0;
    virtual void quitRoom() = 0;
    
    //game functions
    virtual void move(BaseTypes::Move move) = 0;
    virtual void offerDrawGame() = 0;
    virtual void surrender() = 0;
};

#endif /* BaseConnectorProtocol_h */
