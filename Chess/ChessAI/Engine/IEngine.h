//
//  IEngine.h
//  Chess
//
//  Created by Tran Hoang Duong on 9/17/17.
//  Copyright © 2017 Tran Hoang Duong. All rights reserved.
//

#ifndef IEngine_h
#define IEngine_h

#include "BaseTypes.h"
#include <functional>

class IEngine {
public:
    virtual void start(int difficulty) = 0;
    virtual bool move(const BaseTypes::Move& move) = 0;
    virtual void calculate(std::function<void(BaseTypes::Move bestMove)> onCalculated) = 0;
    virtual void log() = 0;
};

#endif /* IEngine_h */
