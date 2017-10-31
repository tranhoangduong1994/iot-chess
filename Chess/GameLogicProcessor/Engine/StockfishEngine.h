//
//  StockfishEngineWrapper.h
//  Chess
//
//  Created by Tran Hoang Duong on 9/17/17.
//  Copyright © 2017 Tran Hoang Duong. All rights reserved.
//

#ifndef StockfishEngine_h
#define StockfishEngine_h

#include "EngineProtocol.h"
#include <string>
#include <map>

#include "types.h"

class ValidatorProtocol;
class StockfishWrapper;
class Position;

class StockfishEngine : public EngineProtocol {
private:
    StockfishWrapper* wrapper;
    ValidatorProtocol* validator;
    
    static StockfishEngine* instance;
    
    void init();
    
public:
    virtual void start(int difficulty) override;
    virtual bool move(const BaseTypes::Move& move) override;
    virtual void calculate(std::function<void(BaseTypes::Move bestMove)> onCalculated) override;
    virtual void log() override;
    
    static StockfishEngine* getInstance();
};

#endif /* StockfishEngineWrapper_h */
