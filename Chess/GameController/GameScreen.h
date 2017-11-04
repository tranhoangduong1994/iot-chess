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
#include <vector>

class OfflineGame;

class GameScreen : public GameEventsProtocol {
public:
    
    void onGameStarted(const EventData& data) override;
    
    void onTurnBegan(const EventData& data) override;
    void onTurnEnded(const EventData& data) override;
    void onInvalidMove(const EventData& data) override;
    
    void onWinGame(const EventData& data) override;
    void onLoseGame(const EventData& data) override;
    void onDrawGame(const EventData& data) override;
    
    static GameScreen* create(OfflineGame* game, int width, int height);
    
private:
    int width;
    int height;
    
    std::vector<std::string> screenBuffer;
    
    OfflineGame* game;
    
    void clearScreen();
    void display(int lineNumber, std::string text);
    std::string getInput();
};

#endif /* GameScreen_h */
