//
//  GameController.h
//  Chess
//
//  Created by macbook on 10/23/17.
//  Copyright © 2017 Tran Hoang Duong. All rights reserved.
//

#ifndef GameController_h
#define GameController_h

#define OFFLINE
#define ONLINE

#include "BaseTypes.h"
#include "GameEventsProtocol.h"

class OfflineGame {
public:
    void start(BaseTypes::Side side, int difficulty);
    void setDelegate(OfflineGameEventsProtocol* delegate);
}

#endif /* GameController_h */
