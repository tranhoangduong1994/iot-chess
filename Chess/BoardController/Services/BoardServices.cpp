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

#include <thread>

const char* PORT = "/dev/ttyUSB0";
const int BAUD = 9600;

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

void BoardServices::awaitService(std::string serviceRequest, std::function<void(EventData)> onFinished) {
    if (!ready) {
        return;
    }
    ready = false;
    
#if defined(__linux) || defined(linux) || defined(__linux)
    serialPuts(fileDescription, serviceRequest.c_str());
    serialFlush(fileDescription);
    bool receivedData = false;
    while(!receivedData) {
        receivedData = serialDataAvail(fileDescription);
    }
#endif
    
#if defined(__APPLE__) || defined(__MACH__)
    std::this_thread::sleep_for(std::chrono::seconds(3));
#endif
    
    EventData eventData;
    onFinished(eventData);
    ready = true;
}

void BoardServices::callService(std::string serviceRequest) {
    std::cout << "BoardServices::callService(" << serviceRequest << ")" << std::endl;
    serviceRequest += "\0";
    if (!ready) {
        std::cout << "Not ready" << std::endl;
        return;
    }
    
#if defined(__linux) || defined(linux) || defined(__linux)
    std::cout << "Calling..." << std::endl;
    serialPuts(fileDescription, serviceRequest.c_str());
    serialFlush(fileDescription);
#endif

}

void BoardServices::awaitSerialPortConnected() {
    std::thread([=]() {
        
#if defined(__linux) || defined(linux) || defined(__linux)
        while (fileDescription < 0) {
            fileDescription = serialOpen(PORT, BAUD);
        }
#endif
        
#if defined(__APPLE__) || defined(__MACH__)
        std::this_thread::sleep_for(std::chrono::seconds(3));
#endif
        
        ready = true;
        if (sDelegate) {
            EventData data;
            data["file_description"] = std::to_string(fileDescription);
            sDelegate->onSerialPortConnected(data);
        }
    }).detach();
}

bool BoardServices::isReady() {
    return ready;
}

void BoardServices::resetGame() {
    std::thread(&BoardServices::awaitService, this, "reset_game", [=](EventData data){
        if (this->gDelegate) {
            gDelegate->onGameReset();
        }
    }).detach();
}

void BoardServices::move(BaseTypes::Move move) {
    std::thread(&BoardServices::awaitService, this, "move," + move.toString(), [=](EventData data){
        if (this->gDelegate) {
            gDelegate->onMotorMoveDone();
        }
    }).detach();
}

void BoardServices::capture(BaseTypes::Move move) {
    std::thread(&BoardServices::awaitService, this, "capture," + move.toString(), [=](EventData data){
        if (this->gDelegate) {
            gDelegate->onMotorMoveDone();
        }
    }).detach();
}

//void BoardServices::scan() {
//    std::thread(&BoardServices::awaitService, this, "scan", [=](EventData data){
//        if (this->gDelegate) {
//            gDelegate->onScanDone(data);
//        }
//    });
//}

void BoardServices::display(int line, std::string string) {
    callService("1" + std::to_string(line - 1) + string);
}

void BoardServices::clearScreen() {
    callService("0");
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
