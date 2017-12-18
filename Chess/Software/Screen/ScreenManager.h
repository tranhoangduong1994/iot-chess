//
//  ScreenManager.h
//  Chess
//
//  Created by macbook on 12/18/17.
//  Copyright © 2017 Tran Hoang Duong. All rights reserved.
//

#ifndef ScreenManager_h
#define ScreenManager_h

class Screen;

class ScreenManager {
public:
    void runScreen(Screen* screen);
    void pushScreen(Screen* screen);
    void popScreen();
    
    static ScreenManager* getInstance();
    
private:
    static ScreenManager* instance;
    static std::vector<Screen*> screenStack;
    
    void init();
};

#endif /* ScreenManager_h */
