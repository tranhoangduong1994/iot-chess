//
//  KeyboardController.h
//  Chess
//
//  Created by macbook on 12/18/17.
//  Copyright © 2017 Tran Hoang Duong. All rights reserved.
//

#ifndef KeyboardController_h
#define KeyboardController_h

#include "KeyboardEventsProtocol.h"

class KeyboardController {
public:
    static KeyboardController* getInstance();
    void setKeyboardEventsDelegator(KeyboardEventsProtocol* delegator);
    
private:
    static KeyboardController* instance;
    KeyboardEventsProtocol* delegator;
    void init();
    void startKeyboardScanning();
    
    bool upPressed;
    bool downPressed;
    bool menuPressed;
    bool okPressed;
};

#endif /* KeyboardController_h */
