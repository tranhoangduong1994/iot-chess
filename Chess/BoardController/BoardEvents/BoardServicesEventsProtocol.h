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
    virtual void onOpponentFinishedMove(BaseTypes::Move move, BaseTypes::Bitboard expectedState) = 0;
    virtual void onScanDone(BaseTypes::Bitboard boardState) = 0;
    virtual void onBoardResetted(BaseTypes::Bitboard currentPhysicsBitboard) = 0;
};

#endif /* BoardServicesEventsProtocol_h */
