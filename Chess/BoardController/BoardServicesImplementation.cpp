//
//  BoardServicesImplementation.cpp
//  Chess
//
//  Created by macbook on 11/4/17.
//  Copyright © 2017 Tran Hoang Duong. All rights reserved.
//

#include "BoardServicesImplementation.h"

//#include <wiringPi.h>
//#include <wiringSerial.h>
#include "wiringPi.h"
#include "wiringSerial.h"

#include <thread>

const char* PORT = "/dev/ttyUSB0";
const int BAUD = 9600;

BoardServicesImplementation* BoardServicesImplementation::instance = NULL;

BoardServicesImplementation* BoardServicesImplementation::getInstance() {
    if (!instance) {
        instance = new BoardServicesImplementation();
        instance->fileDescription = -1;
        instance->ready = false;
        instance->awaitStartingUp(); 
    }
    return instance;
}

void BoardServicesImplementation::awaitService(std::string serviceRequest, std::function<void(EventData)> onFinished) {
    if (!ready) {
        return;
    }
    ready = false;
    serialPuts(fileDescription, serviceRequest.c_str());
    bool receivedData = false;
    while(!receivedData) {
        receivedData = serialDataAvail(fileDescription);
    }
    EventData eventData;
    onFinished(eventData);
    ready = true;
}

void BoardServicesImplementation::callService(std::string serviceRequest) {
    if (!ready) {
        return;
    }
    serialPuts(fileDescription, serviceRequest.c_str());
}

void BoardServicesImplementation::awaitStartingUp() {
    if (fileDescription > -1) {
        return;
    }
    std::thread([=]() {
        while (fileDescription < 0) {
            fileDescription = serialOpen(PORT, BAUD);
        }
        ready = true;
        if (sDelegate) {
            EventData data;
            sDelegate->onStartedUp(data);
        }
    });
}

bool BoardServicesImplementation::isReady() {
    return ready;
}

void BoardServicesImplementation::resetGame() {
    std::thread(&BoardServicesImplementation::awaitService, this, "reset_game", [=](EventData data){
        if (this->gDelegate) {
            gDelegate->onGameReset();
        }
    });
}

void BoardServicesImplementation::move(BaseTypes::Move move) {
    std::thread(&BoardServicesImplementation::awaitService, this, "move," + move.toString(), [=](EventData data){
        if (this->gDelegate) {
            gDelegate->onMoveDone();
        }
    });
}

void BoardServicesImplementation::capture(BaseTypes::Move move) {
    std::thread(&BoardServicesImplementation::awaitService, this, "capture," + move.toString(), [=](EventData data){
        if (this->gDelegate) {
            gDelegate->onMoveDone();
        }
    });
}

void BoardServicesImplementation::scan() {
    std::thread(&BoardServicesImplementation::awaitService, this, "scan", [=](EventData data){
        if (this->gDelegate) {
            gDelegate->onScanDone(data);
        }
    });
}

void BoardServicesImplementation::display(int line, std::string string) {
    callService("display," + std::to_string(line) + "," + string);
}

void BoardServicesImplementation::clearScreen() {
    callService("clear_screen");
}

void BoardServicesImplementation::setBoardSystemEventsDelegate(BoardSystemEventsProtocol* s_delegate) {
    sDelegate = s_delegate;
}

void BoardServicesImplementation::setBoardIngameEventsDelegate(BoardIngameEventsProtocol* g_delegate) {
    gDelegate = g_delegate;
}

void BoardServicesImplementation::setBoardKeyEventsDelegate(BoardKeyEventsProtocol* k_delegate) {
    kDelegate = k_delegate;
}
