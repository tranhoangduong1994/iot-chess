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

struct GameStartedData {
    BaseTypes::Side side;
    GameStartedData(BaseTypes::Side s) {
        side = s;
    }
};

struct TurnBeganData {
    std::string opponent_move;
    TurnBeganData() {};
    TurnBeganData(std::string om) {
        opponent_move = om;
    }
};

struct TurnEndedData {
    std::string player_move;
    TurnEndedData(std::string pm) {
        player_move = pm;
    }
};

struct InvalidMoveData {
    std::string player_move;
    InvalidMoveData(std::string pm) {
        player_move = pm;
    }
};

struct WinGameData {
    std::string player_move;
    WinGameData(std::string pm) {
        player_move = pm;
    }
};

struct LoseGameData {
    std::string opponent_move;
    LoseGameData(std::string om) {
        opponent_move = om;
    }
};

struct DrawGameData {
    std::string final_move;
    DrawGameData(std::string fm) {
        final_move = fm;
    }
};

struct DrawGameOfferedData {
    
};

struct OpponentSurrenderedData {
    
};

class GameEventsProtocol {
public:
    virtual void onBoardInitStateValid() {};
    virtual void onBoardInitStateInvalid(const BaseTypes::Bitboard& misplacedPositions) = 0;
    virtual void onGameStarted(const GameStartedData& data) = 0;
    
    virtual void onMultipleMovesAvailable(const std::vector<BaseTypes::Move>& moves, std::function< void(bool, BaseTypes::Move)> onSelected) = 0;
    
    virtual void onTurnBegan(const TurnBeganData& data) = 0;
    virtual void onTurnEnded(const TurnEndedData& data) = 0;
    virtual void onInvalidMove(const InvalidMoveData& data) = 0;
    
    virtual void onWinGame(const WinGameData& data) = 0;
    virtual void onLoseGame(const LoseGameData& data) = 0;
    virtual void onDrawGame(const DrawGameData& data) = 0;
    
    virtual void onDrawGameOffered(const DrawGameData& data) {};
    virtual void onOpponentSurrendered(const OpponentSurrenderedData& data) {};
};

#endif /* GameEventsProtocol_h */
