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
#include "ScreenManager.h"

bool shouldEnd;

class MainDelegation : public BoardSystemEventsProtocol {
public:
    void onSerialPortConnected(const SerialPortConnectedData& data) {
        std::cout << "[MAIN] MainDelegation - onSerialPortConnected. Port number:  " << data.file_description << "." << std::endl;
        BoardServices::getInstance()->playSound(SoundType::SOUND_SYSTEM_READY);
        ScreenManager::getInstance()->runScreen(MainMenuScreen::create());
    };
    
    void onSerialPortDisconnected() {
        std::cout << "[MAIN] MainDelegation - onSerialPortDisconnected." << std::endl;
        shouldEnd = true;
    };
};

int main(int argc, char* argv[]) {
    shouldEnd = false;
    std::thread([]() {
        BoardServices::getInstance()->setBoardSystemEventsDelegator(new MainDelegation());
    }).detach();
    std::string message = "";
    while (!shouldEnd) {
        std::cin >> message;
        if (message == "exit") {
			break;
		}
		message = "";
    }
    std::cout << "[MAIN] Application has finished running." << std::endl;
    return 0;
};
