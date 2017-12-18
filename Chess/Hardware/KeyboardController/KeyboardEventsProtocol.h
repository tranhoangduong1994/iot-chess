//
//  KeyboardEventsProtocol.h
//  Chess
//
//  Created by macbook on 11/5/17.
//  Copyright © 2017 Tran Hoang Duong. All rights reserved.
//

#ifndef KeyboardEventsProtocol_h
#define KeyboardEventsProtocol_h

#include "BaseTypes.h"

enum BoardKey {
    MENU,
    OK,
    UP,
    DOWN
};

class KeyboardEventsProtocol {
public:
    virtual void onKeyPressed(BoardKey key) = 0;
};

#endif /* KeyboardEventsProtocol_h */
