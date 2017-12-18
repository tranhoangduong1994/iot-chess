//
//  GameEventsProtocol.h
//  Chess
//
//  Created by macbook on 10/22/17.
//  Copyright © 2017 Tran Hoang Duong. All rights reserved.
//

#ifndef GameEventsProtocol_h
#define GameEventsProtocol_h

#include "BaseTypes.h"

#include <functional>

enum DrawGameType {
    STALE_MATE,
    INSUFFICENT_MATERIAL
};

class GameEventsProtocol {
public:
	virtual void onReset() = 0;
    virtual void onBoardInitStateValid() {};
    
    virtual void onGameStarted() = 0;
    
    virtual void onPiecesOffPosition(BaseTypes::Bitboard currentState, BaseTypes::Bitboard expectedState) = 0;
    virtual void onMultipleMovesAvailable(std::vector<BaseTypes::Move> moves, std::function<void(bool, BaseTypes::Move)> onSelected) = 0;
    
    virtual void onGameMenuRequested() = 0;
    
    virtual void onTurnBegan(BaseTypes::Move previousOpponentMove) = 0;
    virtual void onTurnEnded(BaseTypes::Move playerMove) = 0;
    virtual void onInvalidMove() = 0;
    
    virtual void onWinGame(BaseTypes::Move lastMove) = 0;
    virtual void onLoseGame(BaseTypes::Move lastMove) = 0;
    virtual void onDrawGame(DrawGameType type, BaseTypes::Move lastMove) = 0;
    
    //virtual void onDrawGameOffered(const DrawGameData& data) {};
    //virtual void onOpponentSurrendered(const OpponentSurrenderedData& data) {};
};

#endif /* GameEventsProtocol_h */
