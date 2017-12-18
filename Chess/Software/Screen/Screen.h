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

const int SCREEN_WIDTH = 20;
const int SCREEN_HEIGHT = 4;

class Screen {
public:
    virtual void onEnter() {};
    virtual void onExit() {};
    virtual ~Screen() {};
    
    void print(int lineNumber, std::string content);
    void clear();
    void refresh();
    
protected:
    std::vector<std::string> screenBuffer;
    
    Screen();
};

#endif /* Screen_h */
