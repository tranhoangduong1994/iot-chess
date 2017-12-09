//
//  BoardKeyEventsProtocol.h
//  Chess
//
//  Created by macbook on 11/5/17.
//  Copyright © 2017 Tran Hoang Duong. All rights reserved.
//

#ifndef BoardKeyEventsProtocol_h
#define BoardKeyEventsProtocol_h

#include "BaseTypes.h"

enum BoardKey {
    MENU,
    OK,
    UP,
    DOWN
};

class BoardKeyEventsProtocol {
public:
    virtual void onKeyPressed(BoardKey key) = 0;
};

#endif /* BoardKeyEventsProtocol_h */
