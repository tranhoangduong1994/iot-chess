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
#include "BoardKeyEventsProtocol.h"

class MainMenuScreen : public Screen, public BoardKeyEventsProtocol {
public:
    static MainMenuScreen* create(int width, int height);
    void onKeyPressed(const EventData& data) override;
    
private:
    int cursorPositionIndex;
    
    void init();
    void setCursorPosition(int index);
};

#endif /* MainMenuScreen_h */
