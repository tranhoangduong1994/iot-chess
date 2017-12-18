//
//  BoardServicesEventsProtocol
//  Chess
//
//  Created by macbook on 11/5/17.
//  Copyright © 2017 Tran Hoang Duong. All rights reserved.
//

#ifndef BoardServicesEventsProtocol_h
#define BoardServicesEventsProtocol_h

#include "BaseTypes.h"

class BoardServicesEventsProtocol {
public:
    virtual void onOpponentFinishedMove(BaseTypes::Move move, BaseTypes::Bitboard expectedState) {};
    virtual void onScanDone(BaseTypes::Bitboard boardState) {};
    virtual void onBoardResetted(BaseTypes::Bitboard currentPhysicsBitboard) {};
};

#endif /* BoardServicesEventsProtocol_h */
