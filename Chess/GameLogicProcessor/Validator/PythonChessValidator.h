//
//  PythonChessValidator.h
//  Chess
//
//  Created by macbook on 10/10/17.
//  Copyright © 2017 Tran Hoang Duong. All rights reserved.
//

#ifndef PythonChessValidator_h
#define PythonChessValidator_h

#include "ValidatorProtocol.h"
#include <boost/python.hpp>

namespace python = boost::python;

class PythonChessValidator : public ValidatorProtocol {
public:
    virtual void start() override;
    virtual bool checkMove(const BaseTypes::Move& move) override;
    virtual void move(const BaseTypes::Move& move) override;
    
    static PythonChessValidator* getInstance();
    
private:
    python::object validator;
    static PythonChessValidator* instance;
    
    void init();
};

#endif /* PythonChessValidator_h */
