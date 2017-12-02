//
//  MessageController.cpp
//  Chess
//
//  Created by macbook on 11/25/17.
//  Copyright © 2017 Tran Hoang Duong. All rights reserved.
//

#include "MessageController.h"

#include <thread>

#if defined(__linux) || defined(linux) || defined(__linux)
#include <wiringPi.h>
#include <wiringSerial.h>
#endif

#if defined(__APPLE__) || defined(__MACH__)
#include "wiringPi.h"
#include "wiringSerial.h"
#endif

const char* PORT_PREFIX = "/dev/ttyAMA";
const int MAX_PORT_INDEX = 254;
const int BAUD = 9600;

MessageController* MessageController::instance = NULL;

MessageController* MessageController::getInstance() {
    if (!instance) {
        instance = new MessageController();
        instance->init();
    }
    
    return instance;
}

void MessageController::send(ServiceRequestType type, std::string content) {
    serialPuts(fileDescription, (std::to_string(type) + content + "|").c_str());
    serialFlush(fileDescription);
}

void MessageController::checkMessage() {
    char c = serialGetchar(fileDescription);
    if (c == '|') {
        processMessageBuffer();
        messageBuffer = "";
        return;
    }
    messageBuffer += c;
}

void MessageController::processMessageBuffer() {
    if (messageBuffer.at(0) == MessageType::ServiceResponse) {
        if (messageBuffer.at(1) == ServiceResponseType::MOVE_DONE) {
            if (gDelegate) {
                gDelegate->onOpponentFinishedMove(messageBuffer.substr(1));
            }
            return;
        }
        
        if (messageBuffer.at(1) == ServiceResponseType::SCAN_DONE) {
            if (gDelegate) {
                gDelegate->onScanDone(messageBuffer.substr(1));
            }
            return;
        }
        
        if (messageBuffer.at(1) == ServiceResponseType::RESET_DONE) {
            if (gDelegate) {
                gDelegate->onBoardResetted();
            }
            return;
        }
        
        if (messageBuffer.at(1) == ServiceResponseType::PRINT_DONE) {
            
        }
        
        if (messageBuffer.at(1) == ServiceResponseType::CLEAR_SCREEN_DONE) {
            
        }
    } else if (messageBuffer.at(0) == MessageType::Event) {
//        if (messageBuffer.at(1) == EventType::SYSTEM_READY) {
//            if (sDelegate) {
//
//            }
//        }
        if (messageBuffer.at(1) == EventType::BOARD_CHANGED) {
            if (gDelegate) {
                gDelegate->onBoardStateChanged(messageBuffer.substr(2));
            }
        }

        if (messageBuffer.at(1) == EventType::DOWN_PRESSED) {
            if (kDelegate) {
                KeyPressedData data(BoardKey::DOWN);
                kDelegate->onKeyPressed(data);
            }
        }
        
        if (messageBuffer.at(1) == EventType::UP_PRESSED) {
            if (kDelegate) {
                KeyPressedData data(BoardKey::UP);
                kDelegate->onKeyPressed(data);
            }
        }

        
        if (messageBuffer.at(1) == EventType::MENU_PRESSED) {
            if (kDelegate) {
                KeyPressedData data(BoardKey::MENU);
                kDelegate->onKeyPressed(data);
            }
        }
        
        if (messageBuffer.at(1) == EventType::OK_PRESSED) {
            if (kDelegate) {
                KeyPressedData data(BoardKey::OK);
                kDelegate->onKeyPressed(data);
            }
        }
    }
}

void MessageController::init() {
    fileDescription = -1;
    messageBuffer = "";
    
    sDelegate = NULL;
    gDelegate = NULL;
    kDelegate = NULL;
    
    awaitSerialPortConnected();
}

void MessageController::awaitSerialPortConnected() {
    std::thread([=]() {
        int valueToTry = 0;
        std::string portToTry = "";
        while (fileDescription < 0) {
            portToTry = std::string(PORT_PREFIX) + std::to_string(valueToTry);
            valueToTry = (valueToTry + 1) % (MAX_PORT_INDEX + 1);
            std::cout << "Trying port " << portToTry << std::endl;
            fileDescription = serialOpen(portToTry.c_str(), BAUD);
        }
        
        SerialPortConnectedData data(fileDescription);
        sDelegate->onSerialPortConnected(data);
        
        while (true) {
            if (fileDescription == serialOpen(portToTry.c_str(), BAUD) < 0) {
                break;
            }
            checkMessage();
        }
        
        if (sDelegate) {
            sDelegate->onSerialPortDisconnected();
        }
    }).detach();
}

void MessageController::setBoardSystemEventsDelegate(BoardSystemEventsProtocol* s_delegate) {
    sDelegate = s_delegate;
}

void MessageController::setBoardIngameEventsDelegate(BoardIngameEventsProtocol* g_delegate) {
    gDelegate = g_delegate;
}

void MessageController::setBoardKeyEventsDelegate(BoardKeyEventsProtocol* k_delegate) {
    kDelegate = k_delegate;
}
