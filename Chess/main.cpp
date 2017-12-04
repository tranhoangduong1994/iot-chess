//
//  main.cpp
//  Chess
//
//  Created by Tran Hoang Duong on 8/20/17.
//  Copyright © 2017 Tran Hoang Duong. All rights reserved.
//

#include <iostream>
#include <thread>
#include "BoardSystemEventsProtocol.h"
#include "BoardServices.h"
#include "MainMenuScreen.h"
#include "OptionScreen.h"

bool shouldEnd;

class MainDelegation : public BoardSystemEventsProtocol {
public:
    void onSerialPortConnected(const SerialPortConnectedData& data) {
        std::cout << "[MAIN] MainDelegation - onSerialPortConnected. Port number:  " << data.file_description << "." << std::endl;
        Screen::runScreen(MainMenuScreen::create());
    };
    
    void onSerialPortDisconnected() {
        std::cout << "[MAIN] MainDelegation - onSerialPortDisconnected." << std::endl;
        shouldEnd = true;
    };
};

int main(int argc, char* argv[]) {
    shouldEnd = false;
    std::thread([]() {
        BoardServices::getInstance()->setBoardSystemEventsDelegate(new MainDelegation());
    }).detach();
    while (!shouldEnd) {
        std::this_thread::sleep_for(std::chrono::seconds(10));
        std::cout << "[MAIN] Application is running..." << std::endl;
    }
    std::cout << "[MAIN] Application has finished running." << std::endl;
    return 0;
};
