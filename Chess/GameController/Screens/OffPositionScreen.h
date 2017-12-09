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

class OffPositionScreen : public ListScreen, public BoardIngameEventsProtocol {
public:
    static OffPositionScreen* create(const BaseTypes::Bitboard& currentState, const BaseTypes::Bitboard& expectedState);
    
    void onOpponentFinishedMove(const std::string& data, const std::string& newBoardState) override;
    void onScanDone(const std::string& boardState) override;
    void onBoardResetted() override;
    
private:
	OffPositionScreen(std::string header, std::vector<std::string> items);
    void onEnter() override;
    void onExit() override;
    
    void onOKPressed() override;
    
    BaseTypes::Bitboard expectedState;
};

#endif /* OffPositionScreen_h */
