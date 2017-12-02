//
//  MessageController.h
//  Chess
//
//  Created by macbook on 11/25/17.
//  Copyright © 2017 Tran Hoang Duong. All rights reserved.
//

#ifndef MessageController_h
#define MessageController_h

#include <string>

#include "BoardSystemEventsProtocol.h"
#include "BoardIngameEventsProtocol.h"
#include "BoardKeyEventsProtocol.h"

enum MessageType {
    ServiceRequest,
    ServiceResponse,
    Event
};

enum ServiceRequestType {
    CLEAR_SCREEN,
    PRINT,
    MOVE,
    SCAN_BOARD,
    RESET_BOARD
};

enum ServiceResponseType {
    CLEAR_SCREEN_DONE,
    PRINT_DONE,
    MOVE_FAILED,
    MOVE_DONE,
    SCAN_DONE,
    RESET_DONE
};

enum EventType {
    SYSTEM_READY,
    BOARD_CHANGED,
    UP_PRESSED,
    DOWN_PRESSED,
    MENU_PRESSED,
    OK_PRESSED
};

class MessageController {
public:
    void send(ServiceRequestType type, std::string content = "");
    
    static MessageController* getInstance();
    
    void setBoardSystemEventsDelegate(BoardSystemEventsProtocol* s_delegate);
    void setBoardIngameEventsDelegate(BoardIngameEventsProtocol* g_delegate);
    void setBoardKeyEventsDelegate(BoardKeyEventsProtocol* k_delegate);
    
private:
    int fileDescription;
    std::string usbPortNumber;
    std::string messageBuffer;
    
    BoardSystemEventsProtocol* sDelegate;
    BoardIngameEventsProtocol* gDelegate;
    BoardKeyEventsProtocol* kDelegate;
    
    void init();
    
    void awaitSerialPortConnected();
    
    void startLoop();
    
    void checkMessage();
    void processMessageBuffer();
    
    static MessageController* instance;
    MessageController() {}
};

#endif /* MessageController_h */
