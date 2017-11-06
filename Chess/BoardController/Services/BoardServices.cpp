//
//  BoardServicesImplementation.cpp
//  Chess
//
//  Created by macbook on 11/4/17.
//  Copyright © 2017 Tran Hoang Duong. All rights reserved.
//

#include "BoardServices.h"

//#include <wiringPi.h>
//#include <wiringSerial.h>
#include "wiringPi.h"
#include "wiringSerial.h"

#include <thread>

const char* PORT = "/dev/ttyUSB0";
const int BAUD = 9600;

BoardServices* BoardServices::instance = NULL;

BoardServices* BoardServices::getInstance() {
    if (!instance) {
        instance = new BoardServices();
        instance->fileDescription = -1;
        instance->ready = false;
        instance->awaitStartingUp(); 
    }
    return instance;
}

void BoardServices::awaitService(std::string serviceRequest, std::function<void(EventData)> onFinished) {
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

void BoardServices::callService(std::string serviceRequest) {
    if (!ready) {
        return;
    }
    serialPuts(fileDescription, serviceRequest.c_str());
}

void BoardServices::awaitStartingUp() {
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

bool BoardServices::isReady() {
    return ready;
}

void BoardServices::resetGame() {
    std::thread(&BoardServices::awaitService, this, "reset_game", [=](EventData data){
        if (this->gDelegate) {
            gDelegate->onGameReset();
        }
    });
}

void BoardServices::move(BaseTypes::Move move) {
    std::thread(&BoardServices::awaitService, this, "move," + move.toString(), [=](EventData data){
        if (this->gDelegate) {
            gDelegate->onMoveDone();
        }
    });
}

void BoardServices::capture(BaseTypes::Move move) {
    std::thread(&BoardServices::awaitService, this, "capture," + move.toString(), [=](EventData data){
        if (this->gDelegate) {
            gDelegate->onMoveDone();
        }
    });
}

void BoardServices::scan() {
    std::thread(&BoardServices::awaitService, this, "scan", [=](EventData data){
        if (this->gDelegate) {
            gDelegate->onScanDone(data);
        }
    });
}

void BoardServices::display(int line, std::string string) {
    callService("display," + std::to_string(line) + "," + string);
}

void BoardServices::clearScreen() {
    callService("clear_screen");
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
