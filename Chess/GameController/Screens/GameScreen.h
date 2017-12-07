//
//  GameScreen.h
//  Chess
//
//  Created by macbook on 11/2/17.
//  Copyright © 2017 Tran Hoang Duong. All rights reserved.
//

#ifndef GameScreen_h
#define GameScreen_h

#include "GameEventsProtocol.h"
#include "Screen.h"
#include <vector>

class OfflineGame;

class GameScreen : public Screen, public GameEventsProtocol {
public:
    static GameScreen* create(OfflineGame* game);
    
    // GameEventsProtocol implementation
    void onBoardInitStateInvalid(const BaseTypes::Bitboard& offPiecePositions) override;
    void onGameStarted(const GameStartedData& data) override;
    
    void onTurnBegan(const TurnBeganData& data) override;
    void onTurnEnded(const TurnEndedData& data) override;
    void onInvalidMove(const InvalidMoveData& data) override;
    
    void onMultipleMovesAvailable(const std::vector<BaseTypes::Move>& moves, std::function<void(bool, BaseTypes::Move)>) override;
    
    void onWinGame(const WinGameData& data) override;
    void onLoseGame(const LoseGameData& data) override;
    void onDrawGame(const DrawGameData& data) override;
    
protected:
    void onEnter() override;
    void onExit() override;
    
private:
    void init();
    
    ~GameScreen() {}
    OfflineGame* game;
    
    bool entered;
};

#endif /* GameScreen_h */
