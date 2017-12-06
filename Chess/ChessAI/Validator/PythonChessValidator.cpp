//
//  PythonChessValidator.cpp
//  Chess
//
//  Created by macbook on 10/10/17.
//  Copyright © 2017 Tran Hoang Duong. All rights reserved.
//

#include "PythonChessValidator.h"

PythonChessValidator* PythonChessValidator::instance = NULL;

PythonChessValidator* PythonChessValidator::getInstance() {
    if (!instance) {
        instance = new PythonChessValidator();
        instance->init();
    }
    return instance;
}

void PythonChessValidator::init() {
	std::cout << "[PythonChessValidator] init" << std::endl;
    validator = PythonWrapper::getInstance()->createObject("ChessValidator", "Validator");
}

void PythonChessValidator::start() {
    validator.attr("init")();
}

bool PythonChessValidator::checkMove(const BaseTypes::Move& move) {
	std::cout << "[PythonChessValidator] checkMove(" << BaseTypes::Move(move).toString() << ")" << std::endl;
    bool result;
    try {
        result = python::extract<bool>(validator.attr("isMoveValid")(BaseTypes::Move(move).toString().c_str()));
    } catch (const python::error_already_set) {
        PyErr_Print();
        result = false;
    }
    return result;
}

bool PythonChessValidator::checkGameOver() {
	std::cout << "[PythonChessValidator] checkGameOver()" << std::endl;
    bool result;
    try {
        result = python::extract<bool>(validator.attr("isGameOver")());
    } catch (const python::error_already_set) {
        PyErr_Print();
        result = false;
    }
    return result;
}

bool PythonChessValidator::checkDraw() {
	std::cout << "[PythonChessValidator] checkDraw()" << std::endl;
    bool result;
    try {
        result = python::extract<bool>(validator.attr("isDrawGame")());
    } catch (const python::error_already_set) {
        PyErr_Print();
        result = false;
    }
    return result;
}

void PythonChessValidator::move(const BaseTypes::Move& move) {
    if (!checkMove(move)) {
        return;
    }
    try {
        validator.attr("move")(BaseTypes::Move(move).toString().c_str());
    } catch (const python::error_already_set) {
        PyErr_Print();
        assert(false);
    }
}

BaseTypes::Bitboard PythonChessValidator::getAttackedSquares(int attackerSquareIndex) {
    try {
        BaseTypes::Bitboard result(python::extract<std::string>(validator.attr("getAttackedSquares")(attackerSquareIndex)));
        return result;
    } catch (const python::error_already_set) {
        PyErr_Print();
        assert(false);
    }
}
