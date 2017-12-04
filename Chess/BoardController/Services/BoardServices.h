//
//  BoardServices.h
//  Chess
//
//  Created by macbook on 11/4/17.
//  Copyright © 2017 Tran Hoang Duong. All rights reserved.
//

#ifndef BoardServices_h
#define BoardServices_h

#include "IBoardServices.h"
#include <functional>

#include <boost/python.hpp>

#include "BoardSystemEventsProtocol.h"
#include "BoardIngameEventsProtocol.h"
#include "BoardKeyEventsProtocol.h"

class BoardServices : public IBoardServices {
public:
    void resetBoard() override;
    void move(BaseTypes::Move move) override;
    void scan() override;
    
    void display(int line, std::string string) override;
    void display(int line, int position, std::string string) override;
    void clearScreen() override;
    
    void setBoardSystemEventsDelegate(BoardSystemEventsProtocol* s_delegate) override;
    void setBoardIngameEventsDelegate(BoardIngameEventsProtocol* g_delegate) override;
    void setBoardKeyEventsDelegate(BoardKeyEventsProtocol* k_delegate) override;
    
    static BoardServices* getInstance();
    
private:
    static BoardServices* instance;
    
    python::object lcd;
    
    void init();
};


#endif /* BoardServices_h */
