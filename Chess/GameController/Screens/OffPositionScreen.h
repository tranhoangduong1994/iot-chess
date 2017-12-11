//
//  OffPositionScreen.h
//  Chess
//
//  Created by macbook on 12/8/17.
//  Copyright © 2017 Tran Hoang Duong. All rights reserved.
//

#ifndef OffPositionScreen_h
#define OffPositionScreen_h

#include "BoardServicesEventsProtocol.h"

#include "ListScreen.h"

#include <string>

class OffPositionScreen : public ListScreen, public BoardServicesEventsProtocol {
public:
    static OffPositionScreen* create(const BaseTypes::Bitboard& currentState, const BaseTypes::Bitboard& expectedState);
    
    void onScanDone(BaseTypes::Bitboard currentPhysicsBitboard) override;
    
private:
	OffPositionScreen(std::string header, std::vector<std::string> items);
    void onEnter() override;
    void onExit() override;
    
    void onMenuPressed() override;
    void onOKPressed() override;
    
    BaseTypes::Bitboard expectedState;
};

#endif /* OffPositionScreen_h */
