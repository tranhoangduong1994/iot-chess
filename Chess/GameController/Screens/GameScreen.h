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
    static GameScreen* create(OfflineGame* game, int width, int height);
    
    // GameEventsProtocol implementation
    void onGameStarted(const EventData& data) override;
    
    void onTurnBegan(const EventData& data) override;
    void onTurnEnded(const EventData& data) override;
    void onInvalidMove(const EventData& data) override;
    
    void onWinGame(const EventData& data) override;
    void onLoseGame(const EventData& data) override;
    void onDrawGame(const EventData& data) override;
    
private:
    OfflineGame* game;
};

#endif /* GameScreen_h */
