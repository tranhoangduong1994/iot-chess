//
//  BoardServicesImplementation.cpp
//  Chess
//
//  Created by macbook on 11/4/17.
//  Copyright © 2017 Tran Hoang Duong. All rights reserved.
//

#include "BoardServices.h"

#if defined(__linux) || defined(linux) || defined(__linux)
#include <wiringPi.h>
#include <wiringSerial.h>
#endif

#if defined(__APPLE__) || defined(__MACH__)
#include "wiringPi.h"
#include "wiringSerial.h"
#endif

#include <thread>

const char* PORT_PREFIX = "/dev/ttyUSB";
const int MAX_PORT_INDEX = 254;
const int BAUD = 9600;

const char SERVICE_PREFIX_CLEAR_SCREEN = '0';
const char SERVICE_PREFIX_PRINT = '1';
const char SERVICE_PREFIX_MOVE = '2';
const char SERVICE_PREFIX_CAPTURE = '3';
const char SERVICE_PREFIX_SCAN_BOARD = '4';
const char SERVICE_PREFIX_RESET_BOARD = '5';

const char MESSAGE_PREFIX_PLAYER_FINISHED_MOVE = '0';
const char MESSAGE_PREFIX_OPPONENT_FINISHED_MOVE = '1';
const char MESSAGE_PREFIX_KEY_PRESSED = '2';
const char MESSAGE_PREFIX_PLAYER_CHANGED_BOARD_STATE = '3';
const char MESSAGE_PREFIX_BOARD_SCANNED = '4';
const char MESSAGE_PREFIX_BOARD_RESETTED = '5';

BoardServices* BoardServices::instance = NULL;

BoardServices* BoardServices::getInstance() {
    if (!instance) {
        instance = new BoardServices();
        instance->fileDescription = -1;
        instance->ready = false;
        instance->awaitSerialPortConnected();
    }
    return instance;
}

void BoardServices::callService(std::string serviceRequest) {
    std::cout << "BoardServices::callService(" << serviceRequest << ")" << std::endl;
    serviceRequest += "|";
    if (!ready) {
        std::cout << "Not ready" << std::endl;
        return;
    }
    
    serialPuts(fileDescription, serviceRequest.c_str());
    serialFlush(fileDescription);

}

void BoardServices::awaitSerialPortConnected() {
    std::thread([=]() {
        int valueToTry = 0;
        std::string portToTry = "";
        while (fileDescription < 0) {
            portToTry = std::string(PORT_PREFIX) + std::to_string(valueToTry);
            valueToTry = (valueToTry + 1) % (MAX_PORT_INDEX + 1);
            std::cout << "Trying port " << portToTry << std::endl;
            fileDescription = serialOpen(portToTry.c_str(), BAUD);
        }

	std::cout << "SerialPortConnected. Port id = " << portToTry << std::endl;
        
        ready = true;
        if (sDelegate) {
            SerialPortConnectedData data(fileDescription);
            sDelegate->onSerialPortConnected(data);
        }
        
        while(true) {
            if ((fileDescription = serialOpen(portToTry.c_str(), BAUD)) < 0) {
                break;
            }
            loop();
        }
        
        if (sDelegate) {
            sDelegate->onSerialPortDisconnected();
        }
    }).detach();
}

bool BoardServices::isReady() {
    return ready;
}

void BoardServices::clearScreen() {
    callService(std::to_string(SERVICE_PREFIX_CLEAR_SCREEN - 48));
}
                
void BoardServices::display(int line, std::string string) {
    callService(SERVICE_PREFIX_PRINT + std::to_string(line - 1) + string);
}

void BoardServices::move(BaseTypes::Move move) {
    callService(SERVICE_PREFIX_MOVE + move.toString());
}

void BoardServices::capture(BaseTypes::Move move) {
    callService(SERVICE_PREFIX_CAPTURE + move.toString());
}

void BoardServices::scan() {
    callService(std::to_string(SERVICE_PREFIX_SCAN_BOARD - 48));
}

void BoardServices::resetBoard() {
    callService(std::to_string(SERVICE_PREFIX_RESET_BOARD - 48));
}

void BoardServices::loop() {
    char c = serialGetchar(fileDescription);
    if (c == '|') {
        processMessageBuffer();
        messageBuffer = "";
        return;
    }
    messageBuffer += c;
}

void BoardServices::processMessageBuffer() {
    if (messageBuffer.at(0) == MESSAGE_PREFIX_PLAYER_FINISHED_MOVE) {
        if (gDelegate) {
            gDelegate->onPlayerFinishedMove(messageBuffer.substr(1));
        }
        return;
    }
    
    if (messageBuffer.at(0) == MESSAGE_PREFIX_OPPONENT_FINISHED_MOVE) {
        if (gDelegate) {
            gDelegate->onOpponentFinishedMove(messageBuffer.substr(1));
        }
        return;
    }
    
    if (messageBuffer.at(0) == MESSAGE_PREFIX_KEY_PRESSED) {
        if (kDelegate) {
            BoardKey key;
            if (messageBuffer.at(1) == '0') {
                key = BoardKey::UP;
            } else if (messageBuffer.at(1) == '1') {
                key = BoardKey::DOWN;
            } else if (messageBuffer.at(1) == '2') {
                key = BoardKey::MENU;
            } else {
                key = BoardKey::OK;
            }
            KeyPressedData data(key);
            kDelegate->onKeyPressed(data);
        }
        return;
    }
    
    if (messageBuffer.at(0) == MESSAGE_PREFIX_PLAYER_CHANGED_BOARD_STATE) {
        if (gDelegate) {
            gDelegate->onPlayerChangedBoardState(messageBuffer.substr(1));
        }
    }
    
    if (messageBuffer.at(0) == MESSAGE_PREFIX_BOARD_SCANNED) {
        if (gDelegate) {
            gDelegate->onScanDone(messageBuffer.substr(1));
        }
    }
    
    if (messageBuffer.at(0) == MESSAGE_PREFIX_BOARD_RESETTED) {
        if (gDelegate) {
            gDelegate->onBoardResetted();
        }
        return;
    }
}

void BoardServices::setBoardSystemEventsDelegate(BoardSystemEventsProtocol* s_delegate) {
    sDelegate = s_delegate;
}

void BoardServices::setBoardIngameEventsDelegate(BoardIngameEventsProtocol* g_delegate) {
    gDelegate = g_delegate;
}

void BoardServices::setBoardKeyEventsDelegate(BoardKeyEventsProtocol* k_delegate) {
    kDelegate = k_delegate;
}


//void BoardServices::awaitService(std::string serviceRequest, std::function<void(EventData)> onFinished) {
//    if (!ready) {
//        return;
//    }
//    ready = false;
//
//    serialPuts(fileDescription, serviceRequest.c_str());
//    serialFlush(fileDescription);
//    bool receivedData = false;
//    while(!receivedData) {
//        receivedData = serialDataAvail(fileDescription);
//    }
//
//    EventData eventData;
//    onFinished(eventData);
//    ready = true;
//}
