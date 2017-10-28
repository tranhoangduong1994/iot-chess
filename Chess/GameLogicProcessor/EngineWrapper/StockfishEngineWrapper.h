//
//  StockfishEngineWrapper.h
//  Chess
//
//  Created by Tran Hoang Duong on 9/17/17.
//  Copyright © 2017 Tran Hoang Duong. All rights reserved.
//

#ifndef StockfishEngineWrapper_h
#define StockfishEngineWrapper_h

#include "EngineWrapperProtocol.h"
#include <string>
#include <map>

#include "types.h"

class ValidatorProtocol;
class Position;

class StockfishEngineWrapper : public EngineWrapperProtocol {
private:
    std::function<void(BaseTypes::Move bestMove)> onCalculated;
    
    ValidatorProtocol* validator;
    std::string gameMoves;
    static StockfishEngineWrapper* instance;
    
    void init();
    
public:
    virtual void start(int difficulty) override;
    virtual bool move(const BaseTypes::Move& move) override;
    virtual void calculate(std::function<void(BaseTypes::Move bestMove)> onCalculated) override;
    virtual void log() override;
    
    std::function<void(BaseTypes::Move bestMove)>& getCaculatingCallback();
    
    static StockfishEngineWrapper* getInstance();
    static std::string value(Value v);
    static std::string square(Square s);
    static std::string move(Move m, bool chess960);
    static Move to_move(const Position& pos, std::string& str);

};

#endif /* StockfishEngineWrapper_h */
