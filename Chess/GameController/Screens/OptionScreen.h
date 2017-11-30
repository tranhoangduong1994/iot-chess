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
    void onKeyPressed(const KeyPressedData& data) override;
    
protected:
    void onEnter() override;
    void onExit() override;
    
private:
    int cursorPositionIndex;
    bool entered;
    
    void init();
    void setCursorPosition(int index);
    
    void updateScreen();
    
    std::vector<OptionScreenEntry> entries;
    int minEntryIndex;
    int maxEntryIndex;
    
    OptionScreen() {}
};

#endif /* OptionScreen_h */
