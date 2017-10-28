//
//  ValidatorProtocol.h
//  Chess
//
//  Created by Tran Hoang Duong on 9/17/17.
//  Copyright © 2017 Tran Hoang Duong. All rights reserved.
//

#ifndef ValidatorProtocol_h
#define ValidatorProtocol_h

#include "BaseTypes.h"

class ValidatorProtocol {
public:
    virtual void start() = 0;
    virtual bool checkMove(const BaseTypes::Move& move) = 0;
    virtual void move(const BaseTypes::Move& move) = 0;
};

#endif /* ValidatorProtocol_h */
