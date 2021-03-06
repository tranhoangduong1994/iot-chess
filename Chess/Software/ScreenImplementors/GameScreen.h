//
//  GameScreen.h
//  Chess
//
//  Created by macbook on 11/2/17.
//  Copyright © 2017 Tran Hoang Duong. All rights reserved.
//

#ifndef GameScreen_h
#define GameScreen_h

#include "BaseTypes.h"
#include "GameEventsProtocol.h"
#include "Screen.h"
#include <vector>
#include <string>

class GameController;

class GameScreen : public Screen, public GameEventsProtocol {
public:
    static GameScreen* create(GameController* gameController);
    
    // GameEventsProtocol implementation
    void onResetting() override;
    void onPiecesOffPosition(BaseTypes::Bitboard currentState, BaseTypes::Bitboard expectedState) override;
    void onGameStarted() override;
    
    void onTurnBegan(BaseTypes::Move previousOpponentMove) override;
    void onTurnEnded(BaseTypes::Move playerMove) override;
    void onInvalidMove() override;
    
    void onMultipleMovesAvailable(std::vector<BaseTypes::Move> moves, std::function<void(bool, BaseTypes::Move)>) override;
    
    void onPlayerPromotion(std::function<void(std::string pieceType)> onSelected) override;
    
    void onGameMenuRequested() override;
    
    void onWinGame(BaseTypes::Move lastMove) override;
    void onLoseGame(BaseTypes::Move lastMove) override;
    void onDrawGame(DrawGameType type, BaseTypes::Move lastMove) override;
    
protected:
    void onEnter() override;
    void onExit() override;
    
private:
    void init();
    
    std::string getPieceNameByType(BaseTypes::PieceType type);
    
    ~GameScreen() {}
    GameController* gameController;
    
    bool awaitAdjustment;
    
    bool entered;
};

#endif /* GameScreen_h */
