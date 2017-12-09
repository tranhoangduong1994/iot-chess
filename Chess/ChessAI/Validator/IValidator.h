//
//  IValidator.h
//  Chess
//
//  Created by Tran Hoang Duong on 9/17/17.
//  Copyright © 2017 Tran Hoang Duong. All rights reserved.
//

#ifndef IValidator_h
#define IValidator_h

#include "BaseTypes.h"

class IValidator {
public:
    virtual void start() = 0;
    virtual bool checkMove(const BaseTypes::Move& move) = 0;
    virtual bool checkGameOver() = 0;
    virtual bool checkDraw() = 0;
    virtual void move(const BaseTypes::Move& move) = 0;
    virtual BaseTypes::Bitboard getAttackedSquares(int attackerSquareIndex) = 0;
    virtual BaseTypes::Bitboard getBitboard();
};

#endif /* IValidator_h */
