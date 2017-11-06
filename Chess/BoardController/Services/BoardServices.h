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

class BoardServices : public IBoardServices {
public:
    void resetGame() override;
    void move(BaseTypes::Move move) override;
    void capture(BaseTypes::Move move) override;
    void scan() override;
    
    void display(int line, std::string string) override;
    void clearScreen() override;
    
    void setBoardSystemEventsDelegate(BoardSystemEventsProtocol* s_delegate) override;
    void setBoardIngameEventsDelegate(BoardIngameEventsProtocol* g_delegate) override;
    void setBoardKeyEventsDelegate(BoardKeyEventsProtocol* k_delegate) override;
    
    static BoardServices* getInstance();
    
    bool isReady();
    
private:
    int fileDescription;
    bool ready;
    static BoardServices* instance;
    
    BoardSystemEventsProtocol* sDelegate;
    BoardIngameEventsProtocol* gDelegate;
    BoardKeyEventsProtocol* kDelegate;
    
    void awaitService(std::string serviceRequest, std::function<void(EventData)> onFinished);
    void callService(std::string serviceRequest);
    void awaitStartingUp();
};


#endif /* BoardServices_h */
