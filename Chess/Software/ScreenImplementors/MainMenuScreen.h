//
//  MainMenuScreen.h
//  Chess
//
//  Created by macbook on 11/6/17.
//  Copyright © 2017 Tran Hoang Duong. All rights reserved.
//

#ifndef MainMenuScreen_h
#define MainMenuScreen_h

#include "Screen.h"
#include "KeyboardEventsProtocol.h"

class MainMenuScreen : public Screen, public KeyboardEventsProtocol {
public:
    static MainMenuScreen* create();
    void onKeyPressed(BoardKey key) override;
    
protected:
    void onEnter() override;
    void onExit() override;
    
private:
    int cursorPositionIndex;
    bool entered;
    
    void init();
    void setCursorPosition(int index);
    
    ~MainMenuScreen() {}
};

#endif /* MainMenuScreen_h */
