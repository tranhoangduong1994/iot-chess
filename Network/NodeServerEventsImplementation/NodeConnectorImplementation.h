//
//  NodeConnectorImplementation.h
//  Chess
//
//  Created by macbook on 10/22/17.
//  Copyright © 2017 Tran Hoang Duong. All rights reserved.
//

#ifndef NodeConnectorImplementation_h
#define NodeConnectorImplementation_h

#include "BaseTypes.h"
#include "BaseConnectorProtocol.h"
#include "ConnectingEventsProtocol.h"
#include "GameEventsProtocol.h"
#include "RoomEventsProtocol.h"

#include "sio_client.h"

class NodeConnectorImplementation : public BaseConnectorProtocol {
private:
    ConnectingEventsProtocol* connectingDelegate = NULL;
    RoomEventsProtocol* roomDelegate = NULL;
    GameEventsProtocol* gameDelegate = NULL;
    
    sio::client* client;
    void init();
    
    static NodeConnectorImplementation* instance;
    
public:
    virtual void connect(std::string host) override;
    virtual void login(std::string username, std::string password) override;
    
    virtual void createRoom(std::string roomName, BaseTypes::Side side) override;
    virtual void joinRoom(std::string roomId) override;
    virtual void quitRoom() override;
    
    virtual void move(BaseTypes::Move move) override;
    virtual void offerDrawGame() override;
    virtual void surrender() override;
    
    virtual void setConnectingEventsDelegate(ConnectingEventsProtocol* delegate) override;
    virtual void setRoomEventsDelegate(RoomEventsProtocol* delegate) override;
    virtual void setGameEventsDelegate(GameEventsProtocol* delegate) override;
    
    static NodeConnectorImplementation* getInstance();
};

#endif /* NodeConnectorImplementation_h */
