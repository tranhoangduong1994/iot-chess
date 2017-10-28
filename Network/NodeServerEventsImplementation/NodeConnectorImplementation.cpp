//
//  NodeConnectorImplementation.cpp
//  Chess
//
//  Created by macbook on 10/22/17.
//  Copyright © 2017 Tran Hoang Duong. All rights reserved.
//

#include "NodeConnectorImplementation.h"

NodeConnectorImplementation* NodeConnectorImplementation::instance = NULL;

NodeConnectorImplementation* NodeConnectorImplementation::getInstance() {
    if (!instance) {
        instance = new NodeConnectorImplementation();
        instance->init();
    }
    return instance;
}

void NodeConnectorImplementation::connect(std::string host) {
    client->connect(host);
}

void NodeConnectorImplementation::login(std::string username, std::string password) {
    auto msg = sio::object_message::create();
    msg->get_map()["username"] = sio::string_message::create(username);
    msg->get_map()["password"] = sio::string_message::create(password);
    client->socket()->emit("login", msg);
}

void NodeConnectorImplementation::createRoom(std::string roomName, BaseTypes::Side side) {
    std::string sideString = (side == BaseTypes::Side::WHITE ? "WHITE" : "BLACK");
    auto msg = sio::object_message::create();
    msg->get_map()["roomName"] = sio::string_message::create(roomName);
    msg->get_map()["side"] = sio::string_message::create(sideString);
    client->socket()->emit("createRoom", msg);
}

void NodeConnectorImplementation::joinRoom(std::string roomId) {
    auto msg = sio::object_message::create();
    msg->get_map()["roomId"] = sio::string_message::create(roomId);
    client->socket()->emit("joinRoom", msg);
}

void NodeConnectorImplementation::quitRoom() {
    client->socket()->emit("quitRoom");
}

void NodeConnectorImplementation::move(BaseTypes::Move move) {
    auto msg = sio::object_message::create();
    msg->get_map()["move"] = sio::string_message::create(move.toString());
    client->socket()->emit("move", msg);
}

void NodeConnectorImplementation::offerDrawGame() {
    client->socket()->emit("offerDrawGame");
}

void NodeConnectorImplementation::surrender() {
    client->socket()->emit("surrender");
}

void NodeConnectorImplementation::init() {
    client = new sio::client();
    
    //CONNECTING EVENTS
    client->set_socket_open_listener([&](std::string nsp) {
        EventData data;
        connectingDelegate->onConnected(data);
    });
    
    client->socket()->on("loginSucceeded", [&](const std::string& name,sio::message::ptr const& message,bool need_ack, sio::message::list& ack_message) {
        EventData data;
        connectingDelegate->onLoggedInSucceeded(data);
    });
    
    client->socket()->on("loginFailed", [&](const std::string& name,sio::message::ptr const& message,bool need_ack, sio::message::list& ack_message) {
        EventData data;
        data["error"] = message->get_map()["error"]->get_string();
        connectingDelegate->onLoggedInFailed(data);
    });
    
    //ROOM EVENTS
    client->socket()->on("createRoomSucceeded", [&](const std::string& name,sio::message::ptr const& message,bool need_ack, sio::message::list& ack_message) {
        EventData data;
        data["roomId"] = message->get_map()["roomId"]->get_string();
        roomDelegate->onRoomCreatedSucceeded(data);
    });
    
    client->socket()->on("createRoomFailed", [&](const std::string& name,sio::message::ptr const& message,bool need_ack, sio::message::list& ack_message) {
        EventData data;
        data["error"] = message->get_map()["error"]->get_string();
        roomDelegate->onRoomCreatedFailed(data);
    });
    
    client->socket()->on("joinedRoomSucceeded", [&](const std::string& name,sio::message::ptr const& message,bool need_ack, sio::message::list& ack_message) {
        EventData data;
        roomDelegate->onJoinedRoomSucceeded(data);
    });
    
    client->socket()->on("joinedRoomFailed", [&](const std::string& name,sio::message::ptr const& message,bool need_ack, sio::message::list& ack_message) {
        EventData data;
        data["error"] = message->get_map()["error"]->get_string();
        roomDelegate->onJoinedRoomFailed(data);
    });
    
    client->socket()->on("opponentJoined", [&](const std::string& name,sio::message::ptr const& message,bool need_ack, sio::message::list& ack_message) {
        EventData data;
        data["opponentUsername"] = message->get_map()["opponentUsername"]->get_string();
        roomDelegate->onOpponentJoined(data);
    });
    
    client->socket()->on("opponentQuited", [&](const std::string& name,sio::message::ptr const& message,bool need_ack, sio::message::list& ack_message) {
        EventData data;
        roomDelegate->onOpponentQuited(data);
    });
    
    client->socket()->on("opponentDisconnected", [&](const std::string& name,sio::message::ptr const& message,bool need_ack, sio::message::list& ack_message) {
        EventData data;
        roomDelegate->onOpponentDisconnected(data);
    });
    
    //GAME EVENTS
    client->socket()->on("gameStarted", [&](const std::string& name,sio::message::ptr const& message,bool need_ack, sio::message::list& ack_message) {
        EventData data;
        gameDelegate->onGameStarted(data);
    });
    
    client->socket()->on("turnBegan", [&](const std::string& name,sio::message::ptr const& message,bool need_ack, sio::message::list& ack_message) {
        EventData data;
        data["lastMove"] = message->get_map()["lastMove"]->get_string();
        gameDelegate->onTurnBegan(data);
    });
    
    client->socket()->on("turnEnded", [&](const std::string& name,sio::message::ptr const& message,bool need_ack, sio::message::list& ack_message) {
        EventData data;
        gameDelegate->onTurnEnded(data);
    });
    
    client->socket()->on("invalidMove", [&](const std::string& name,sio::message::ptr const& message,bool need_ack, sio::message::list& ack_message) {
        EventData data;
        gameDelegate->onInvalidMove(data);
    });
    
    client->socket()->on("drawGameOffered", [&](const std::string& name,sio::message::ptr const& message,bool need_ack, sio::message::list& ack_message) {
        EventData data;
        gameDelegate->onDrawGameOffered(data);
    });
    
    client->socket()->on("opponentSurrendered", [&](const std::string& name,sio::message::ptr const& message,bool need_ack, sio::message::list& ack_message) {
        EventData data;
        gameDelegate->onOpponentSurrendered(data);
    });
    
    client->socket()->on("winGame", [&](const std::string& name,sio::message::ptr const& message,bool need_ack, sio::message::list& ack_message) {
        EventData data;
        gameDelegate->onWinGame(data);
    });
    
    client->socket()->on("loseGame", [&](const std::string& name,sio::message::ptr const& message,bool need_ack, sio::message::list& ack_message) {
        EventData data;
        gameDelegate->onLoseGame(data);
    });
    
    client->socket()->on("drawGame", [&](const std::string& name,sio::message::ptr const& message,bool need_ack, sio::message::list& ack_message) {
        EventData data;
        gameDelegate->onDrawGame(data);
    });
}

void NodeConnectorImplementation::setConnectingEventsDelegate(ConnectingEventsProtocol* delegate) {
    connectingDelegate = delegate;
}

void NodeConnectorImplementation::setRoomEventsDelegate(RoomEventsProtocol* delegate) {
    roomDelegate = delegate;
}

void NodeConnectorImplementation::setGameEventsDelegate(GameEventsProtocol* delegate) {
    gameDelegate = delegate;
}
