//
//  ListScreen.h
//  Chess
//
//  Created by macbook on 12/7/17.
//  Copyright © 2017 Tran Hoang Duong. All rights reserved.
//

#ifndef ListScreen_h
#define ListScreen_h

#include <string>
#include <vector>
#include <functional>

#include "BoardKeyEventsProtocol.h"
#include "Screen.h"

class ListScreen: public Screen, public BoardKeyEventsProtocol {
public:
    static ListScreen* create(std::string header, std::vector<std::string> items);
    void onKeyPressed(const KeyPressedData& data) override;
    
protected:
    void onEnter() override;
    void onExit() override;
    
    void updateScreen();
    void updateList(const std::vector<std::string>& items);
    std::vector<std::string> items;
    
private:
    int currentPage;
    std::string header;
    
    void init();
    ListScreen() {}
};

#endif /* ListScreen_h */

