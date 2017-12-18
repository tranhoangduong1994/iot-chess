//
//  GameState.h
//  Chess
//
//  Created by macbook on 12/9/17.
//  Copyright © 2017 Tran Hoang Duong. All rights reserved.
//

#ifndef GameState_h
#define GameState_h

#include "BaseTypes.h"

class GameController;

class GameState {
public:
    virtual void handleKey(GameController* game, BoardKey key) {}
    virtual void handleBoardScanningResult(GameController* game, BaseTypes::Bitboard newBoard) {}
};

#endif /* GameState_h */
