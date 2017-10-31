//
//  StockfishWrapper.h
//  Chess
//
//  Created by macbook on 10/30/17.
//  Copyright © 2017 Tran Hoang Duong. All rights reserved.
//

#ifndef StockfishWrapper_h
#define StockfishWrapper_h

#include "types.h"
#include "position.h"
#include "BaseTypes.h"

#include <string>

class StockfishWrapper {
private:
    std::string gameMoves;
    std::function<void(BaseTypes::Move bestMove)> onCalculated;
    static StockfishWrapper* instance;
    void init();
    
public:
    void start(int difficulty);
    void move(BaseTypes::Move move);
    void calculate(std::function<void(BaseTypes::Move bestMove)> onCalculated);
    void log();
    
    static std::string value(Value v);
    static std::string square(Square s);
    static std::string move(Move m, bool chess960);
    static Move to_move(const Position& pos, std::string& str);
    
    std::function<void(BaseTypes::Move bestMove)>& getCaculatingCallback();
    
    static StockfishWrapper* getInstance();
};

#endif /* StockfishWrapper_h */
