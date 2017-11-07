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

class MainDelegation : public BoardSystemEventsProtocol {
public:
    void onSerialPortConnected(const EventData& data) {
        std::cout << "MainDelegation - onSerialPortConnected: " << data.at("file_description") << std::endl;
        Screen::pushScreen(MainMenuScreen::create(20, 4));
    };
};

int main(int argc, char* argv[]) {
    std::cout << "Start" << std::endl;
    std::thread([]() {
        BoardServices::getInstance()->setBoardSystemEventsDelegate(new MainDelegation());
    }).detach();
    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::cout << "running..." << std::endl;
    }
    return 0;
}

//#include <stdio.h>
//#include <iostream>
//
//const int CMD_BUFFER_LENGTH = 102;
//const int STRING_LENGTH = 100;
//
//char commandBuffer[CMD_BUFFER_LENGTH];
//
//void tryPerformingCommand() {
//    if (commandBuffer[0] == '0') {
//        //clear screen
//        std::cout << "lcd.clear();" << std::endl;
//    } else if (commandBuffer[0] == '1') {
//        //print
//        char line = commandBuffer[1] - 48;
//        std::cout << "lcd.setCursor(0, " + std::to_string(line) + ");" << std::endl;
//        char stringBuffer[STRING_LENGTH] = "";
//        int i = 2;
//        while(true) {
//            char character = commandBuffer[i];
//            if (character == 0) break;
//            stringBuffer[i - 2] = character;
//            i++;
//        }
//        std::cout << stringBuffer << std::endl;
//    }
//}
//
//void clearCommandBuffer() {
//    for (int i = 0; i < CMD_BUFFER_LENGTH; i++) {
//        commandBuffer[i] = 0;
//    }
//}
//
//int main(int argc, char* argv[]) {
//    clearCommandBuffer();
////    std::cout << commandBuffer << std::endl;
//    strncpy(commandBuffer, "0", 100);
//    tryPerformingCommand();
//
//    clearCommandBuffer();
////    std::cout << commandBuffer << std::endl;
//    strncpy(commandBuffer, "10Hello, world", 100);
//    tryPerformingCommand();
//
//    return 0;
//}

