//
//  GameSettingScreen.h
//  Chess
//
//  Created by macbook on 12/11/17.
//  Copyright © 2017 Tran Hoang Duong. All rights reserved.
//

#ifndef GameSettingScreen_h
#define GameSettingScreen_h

#include "BaseTypes.h"
#include "OptionScreen.h"

class GameSettingScreen : public OptionScreen {
public:
    static GameSettingScreen* create();
    
private:
    GameSettingScreen();
    
    BaseTypes::Side side;
    int difficulty;
    virtual void onEnter() override;
    virtual void onExit() override;
    
    virtual void init() override;
};

#endif /* GameSettingScreen_h */
