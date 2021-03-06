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

#include "KeyboardEventsProtocol.h"
#include "Screen.h"

class ListScreen: public Screen, public KeyboardEventsProtocol {
public:
    static ListScreen* create(std::string header, std::vector<std::string> items);
    void onKeyPressed(BoardKey key) override;
    
protected:
	ListScreen(std::string header, std::vector<std::string> items);

    virtual void onEnter() override;
    virtual void onExit() override;
    
    void updateScreen();
    void updateList(const std::vector<std::string>& items);
    std::vector<std::string> items;
    
    virtual void onMenuPressed();
    virtual void onOKPressed();
    virtual void onUpPressed();
    virtual void onDownPressed();
    
private:
	ListScreen() {}
	void init();

    int currentPage;
    std::string header;
};

#endif /* ListScreen_h */

