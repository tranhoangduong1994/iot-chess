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
#include "BoardServicesEventsProtocol.h"

enum MessageType {
    ServiceRequest,
    ServiceResponse,
    Event
};

enum ServiceRequestType {
    MOVE,
    CASTLING,
    SCAN_BOARD,
    RESET_BOARD,
    PLAY_SOUND
};

enum ServiceResponseType {
    MOVE_FAILED,
    MOVE_DONE,
    SCAN_DONE,
    RESET_DONE
};

enum EventType {
    SYSTEM_READY,
    BOARD_CHANGED
};

class MessageController {
public:
    void send(ServiceRequestType type, std::string content = "");
    void setBoardSystemEventsDelegator(BoardSystemEventsProtocol* delegator);
    void setBoardServicesEventsDelegator(BoardServicesEventsProtocol* delegator);
    
    static MessageController* getInstance();
    
private:
    int fileDescription;
    std::string usbPortNumber;
    std::string messageBuffer;
    
    BoardSystemEventsProtocol* sDelegator;
    BoardServicesEventsProtocol* gDelegator;
    
    void init();
    
    void initSerialPort();
    
    void startLoop();
    void startKeyboardScanning();
    
    void checkMessage();
    void processMessageBuffer();
    
    static MessageController* instance;
    MessageController() {}
};

#endif /* MessageController_h */
