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
                if (!digitalRead(6)) {
                    upPressed = true;
                    if (delegator) {
                        delegator->onKeyPressed(BoardKey::UP);
                    }
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }
            if (digitalRead(6) && upPressed) {
                if (digitalRead(6)) {
                    upPressed = false;
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }
            
            if (!digitalRead(13) && !upPressed && !downPressed && !okPressed && !menuPressed) {
                if (!digitalRead(13)) {
                    downPressed = true;
                    if (delegator) {
                        delegator->onKeyPressed(BoardKey::DOWN);
                    }
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }
            if (digitalRead(13) && downPressed) {
                if (digitalRead(13)) {
                    downPressed = false;
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }
            
            if (!digitalRead(19) && !upPressed && !downPressed && !okPressed && !menuPressed) {
                if (!digitalRead(19)) {
                    menuPressed = true;
                    if (delegator) {
                        delegator->onKeyPressed(BoardKey::MENU);
                    }
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }
            if (digitalRead(19) && menuPressed) {
                if (digitalRead(19)) {
                    menuPressed = false;
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }
            
            if (!digitalRead(26) && !upPressed && !downPressed && !okPressed && !menuPressed) {
                if (!digitalRead(26)) {
                    okPressed = true;
                    if (delegator) {
                        delegator->onKeyPressed(BoardKey::OK);
                    }
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }
            if (digitalRead(26) && okPressed) {
                if (digitalRead(26)) {
                    okPressed = false;
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }
        }
    }).detach();
}

void KeyboardController::setKeyboardEventsDelegator(KeyboardEventsProtocol *delegator) {
    this->delegator = delegator;
}
