//
//  MessageController.cpp
//  Chess
//
//  Created by macbook on 11/25/17.
//  Copyright © 2017 Tran Hoang Duong. All rights reserved.
//

#include "MessageController.h"
#include "BoardServices.h"
#include <thread>

#if defined(__linux) || defined(linux) || defined(__linux)
#include <wiringPi.h>
#include <wiringSerial.h>
#endif

#if defined(__APPLE__) || defined(__MACH__)
#include "wiringPi.h"
#include "wiringSerial.h"
#endif

const char* PORT_PREFIX = "/dev/ttyUSB";
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
	std::cout << "[MessageController] send(" << type << ", " << content << ")" << std::endl;
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
	std::cout << "[MessageController] processMessageBuffer: <--" << messageBuffer <<  "-->" << std::endl;
    int messageType = messageBuffer.at(0) - 48;
    int messageHeader = messageBuffer.at(1) - 48;
    if (messageType == MessageType::ServiceResponse) {
        if (messageHeader == ServiceResponseType::MOVE_DONE) {
            if (gDelegate) {
                gDelegate->onOpponentFinishedMove(messageBuffer.substr(1));
            }
            return;
        }
        
        if (messageHeader == ServiceResponseType::SCAN_DONE) {
            if (gDelegate) {
                gDelegate->onScanDone(messageBuffer.substr(1));
            }
            return;
        }
        
        if (messageHeader == ServiceResponseType::RESET_DONE) {
            if (gDelegate) {
                gDelegate->onBoardResetted();
            }
            return;
        }
        
        if (messageHeader == ServiceResponseType::PRINT_DONE) {
            
        }
        
        if (messageHeader == ServiceResponseType::CLEAR_SCREEN_DONE) {
            
        }
    } else if (messageType == MessageType::Event) {
        if (messageHeader == EventType::SYSTEM_READY) {
            if (sDelegate) {
                SerialPortConnectedData data(fileDescription);
                sDelegate->onSerialPortConnected(data);
            }
        }
        if (messageHeader == EventType::BOARD_CHANGED) {
            if (gDelegate) {
                gDelegate->onBoardStateChanged(messageBuffer.substr(2));
            }
        }

        if (messageHeader == EventType::DOWN_PRESSED) {
            if (kDelegate) {
                KeyPressedData data(BoardKey::DOWN);
                kDelegate->onKeyPressed(data);
            }
        }
        
        if (messageHeader == EventType::UP_PRESSED) {
            if (kDelegate) {
                KeyPressedData data(BoardKey::UP);
                kDelegate->onKeyPressed(data);
            }
        }
        
        if (messageHeader == EventType::MENU_PRESSED) {
            if (kDelegate) {
                KeyPressedData data(BoardKey::MENU);
                kDelegate->onKeyPressed(data);
            }
        }
        
        if (messageHeader == EventType::OK_PRESSED) {
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
        usbPortNumber = "";
        while (fileDescription < 0) {
            usbPortNumber = std::string(PORT_PREFIX) + std::to_string(valueToTry);
            valueToTry = (valueToTry + 1) % (MAX_PORT_INDEX + 1);
            std::cout << "Trying port " << usbPortNumber << std::endl;
            fileDescription = serialOpen(usbPortNumber.c_str(), BAUD);
        }
	startLoop();
    }).detach();
}

void MessageController::startLoop() {
    while (true) {
        if (fileDescription == serialOpen(usbPortNumber.c_str(), BAUD) < 0) {
            break;
        }
        checkMessage();
    }
        
    if (sDelegate) {
        sDelegate->onSerialPortDisconnected();
    }
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
