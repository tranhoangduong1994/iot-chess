//
//  Screen.h
//  Chess
//
//  Created by macbook on 11/5/17.
//  Copyright © 2017 Tran Hoang Duong. All rights reserved.
//

#ifndef Screen_h
#define Screen_h

#include <string>
#include <vector>

#include "DisplayerImplementation.h"

class Screen {
public:
    static void pushScreen(Screen* screen);
    static void popScreen();
    static void replaceScreen(Screen* screen);
    
    void print(int lineNumber, std::string content);
    void clearScreen();
    
protected:
    static std::vector<Screen*> screenStack;
    static IDisplayer* displayer;
    static Screen* currentScreen;
    
    int width, height;
    std::vector<std::string> screenBuffer;
    
    Screen() {};
    virtual ~Screen() {};
    
    virtual void onEnter() {};
    virtual void onExit() {};
private:
    static void runScreen(Screen* screen);
};

#endif /* Screen_h */
