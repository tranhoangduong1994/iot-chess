//
//  PythonChessValidator.h
//  Chess
//
//  Created by macbook on 10/10/17.
//  Copyright © 2017 Tran Hoang Duong. All rights reserved.
//

#ifndef PythonChessValidator_h
#define PythonChessValidator_h

#include "IValidator.h"

#include "PythonWrapper.h"

class PythonChessValidator : public IValidator {
public:
    virtual void start() override;
    virtual bool checkMove(const BaseTypes::Move& move) override;
    virtual bool checkGameOver() override;
    virtual bool checkDraw() override;
    virtual void move(const BaseTypes::Move& move) override;
    
    virtual BaseTypes::Bitboard getAttackedSquares(int attackerSquareIndex) override;
    
    static PythonChessValidator* getInstance();
    
    ~PythonChessValidator() {};
    
private:
    python::object validator;
    static PythonChessValidator* instance;
    
    void init();
};

#endif /* PythonChessValidator_h */
