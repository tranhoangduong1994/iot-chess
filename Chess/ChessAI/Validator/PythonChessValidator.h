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
    void start() override;
    bool checkMove(const BaseTypes::Move& move) override;
    bool checkGameOver() override;
    bool checkDraw() override;
    void move(const BaseTypes::Move& move) override;
    
    BaseTypes::Bitboard getAttackedSquares(int attackerSquareIndex) override;
    BaseTypes::Bitboard getBitboard() override;
    
    static PythonChessValidator* getInstance();
    
    ~PythonChessValidator() {};
    
private:
    python::object validator;
    static PythonChessValidator* instance;
    
    void init();
};

#endif /* PythonChessValidator_h */
