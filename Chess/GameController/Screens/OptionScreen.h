//
//  ListScreen.h
//  Chess
//
//  Created by macbook on 11/29/17.
//  Copyright © 2017 Tran Hoang Duong. All rights reserved.
//

#ifndef OptionScreen_h
#define OptionScreen_h

#include "Screen.h"

#include <functional>

#include "BoardKeyEventsProtocol.h"

struct OptionScreenEntry {
    std::string name;
    std::function<void(std::string content)> onSelected;
};

class OptionScreen : public Screen, public BoardKeyEventsProtocol {
public:
    static OptionScreen* create(std::string header, std::vector<OptionScreenEntry> entries);
    void onKeyPressed(BoardKey key) override;
    
protected:
    OptionScreen(std::string header, std::vector<OptionScreenEntry> entries);
    virtual void onEnter() override;
    virtual void onExit() override;
    std::vector<OptionScreenEntry> entries;
    
    virtual void init();
    
private:
    int cursorPositionIndex;
    bool entered;
    std::string header;
    
    void setCursorPosition(int index);
    
    void updateScreen();
    
    int minEntryIndex;
    int maxEntryIndex;
};

#endif /* OptionScreen_h */
