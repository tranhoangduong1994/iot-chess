//
//  KeyboardController.cpp
//  Chess
//
//  Created by macbook on 12/18/17.
//  Copyright © 2017 Tran Hoang Duong. All rights reserved.
//

#include "KeyboardController.h"
#include <thread>

#if defined(__linux) || defined(linux) || defined(__linux)
#include <wiringPi.h>
#include <wiringSerial.h>
#endif

#if defined(__APPLE__) || defined(__MACH__)
#include "wiringPi.h"
#include "wiringSerial.h"
#endif

KeyboardController* KeyboardController::instance = NULL;

KeyboardController* KeyboardController::getInstance() {
    if (!instance) {
        instance = new KeyboardController();
        instance->init();
    }
    return instance;
}

void KeyboardController::init() {
    wiringPiSetup();
    wiringPiSetupGpio();
    
    upPressed = false;
    downPressed = false;
    menuPressed = false;
    okPressed = false;
    
    pullUpDnControl(6, PUD_UP);
    pullUpDnControl(13, PUD_UP);
    pullUpDnControl(19, PUD_UP);
    pullUpDnControl(26, PUD_UP);
    
    startKeyboardScanning();
}

void KeyboardController::startKeyboardScanning() {
    std::thread([=]() {
        while(true) {
            if (!digitalRead(6) && !upPressed && !downPressed && !okPressed && !menuPressed) {
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
                if (!digitalRead(6)) {
                    upPressed = true;
                    if (delegator) {
                        delegator->onKeyPressed(BoardKey::UP);
                    }
                }
            }
            if (digitalRead(6) && upPressed) {
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
                if (digitalRead(6)) {
                    upPressed = false;
                }
            }
            
            if (!digitalRead(13) && !upPressed && !downPressed && !okPressed && !menuPressed) {
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
                if (!digitalRead(13)) {
                    downPressed = true;
                    if (delegator) {
                        delegator->onKeyPressed(BoardKey::DOWN);
                    }
                }
            }
            if (digitalRead(13) && downPressed) {
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
                if (digitalRead(13)) {
                    downPressed = false;
                }
            }
            
            if (!digitalRead(19) && !upPressed && !downPressed && !okPressed && !menuPressed) {
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
                if (!digitalRead(19)) {
                    menuPressed = true;
                    if (delegator) {
                        delegator->onKeyPressed(BoardKey::MENU);
                    }
                }
            }
            if (digitalRead(19) && menuPressed) {
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
                if (digitalRead(19)) {
                    menuPressed = false;
                }
            }
            
            if (!digitalRead(26) && !upPressed && !downPressed && !okPressed && !menuPressed) {
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
                if (!digitalRead(26)) {
                    okPressed = true;
                    if (delegator) {
                        delegator->onKeyPressed(BoardKey::OK);
                    }
                }
            }
            if (digitalRead(26) && okPressed) {
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
                if (digitalRead(26)) {
                    okPressed = false;
                }
            }
        }
    }).detach();
}

void KeyboardController::setKeyboardEventsDelegator(KeyboardEventsProtocol *delegator) {
    this->delegator = delegator;
}
