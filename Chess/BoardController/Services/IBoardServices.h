//
//  IBoardServices.h
//  Chess
//
//  Created by macbook on 11/4/17.
//  Copyright © 2017 Tran Hoang Duong. All rights reserved.
//

#ifndef IBoardServices_h
#define IBoardServices_h

#include "BaseTypes.h"

#include "BoardSystemEventsProtocol.h"
#include "BoardServicesEventsProtocol.h"
#include "BoardKeyEventsProtocol.h"

enum CastlingType {
	WHITE_QUEEN_SIDE,
	WHITE_KING_SIDE,
	BLACK_QUEEN_SIDE,
	BLACK_KING_SIDE
};

class IBoardServices {
public:
    virtual void resetBoard() = 0;
    virtual void move(BaseTypes::Move move) = 0;
    virtual void castling(CastlingType type) = 0;
    virtual void scan() = 0;
    
    virtual void display(int line, std::string string) = 0;
    virtual void display(int line, int position, std::string string) = 0;
    virtual void clearScreen() = 0;
    
    virtual void setBoardSystemEventsDelegate(BoardSystemEventsProtocol* s_delegate) = 0;
    virtual void setBoardServicesEventsDelegate(BoardServicesEventsProtocol* g_delegate) = 0;
    virtual void setBoardKeyEventsDelegate(BoardKeyEventsProtocol* k_delegate) = 0;
};

#endif /* IBoardServices_h */
