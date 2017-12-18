//
//  BoardServices.h
//  Chess
//
//  Created by macbook on 11/4/17.
//  Copyright © 2017 Tran Hoang Duong. All rights reserved.
//

#ifndef BoardServices_h
#define BoardServices_h

#include <functional>

#include "BoardSystemEventsProtocol.h"
#include "BoardServicesEventsProtocol.h"

enum CastlingType {
    WHITE_QUEEN_SIDE,
    WHITE_KING_SIDE,
    BLACK_QUEEN_SIDE,
    BLACK_KING_SIDE
};

class BoardServices {
public:
    void resetBoard();
    void move(BaseTypes::Move move);
    void castling(CastlingType type);
    void scan();
    
    void setBoardSystemEventsDelegator(BoardSystemEventsProtocol* delegator);
    void setBoardServicesEventsDelegator(BoardServicesEventsProtocol* delegator);
    
    static BoardServices* getInstance();
    
private:
    static BoardServices* instance;
    
    void init();
};


#endif /* BoardServices_h */