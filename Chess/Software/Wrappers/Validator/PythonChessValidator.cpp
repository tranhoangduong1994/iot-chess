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
    validator = PythonHelper::getInstance()->getObject("ChessValidator", "Validator");
}

void PythonChessValidator::start() {
    validator.attr("init")();
}

bool PythonChessValidator::checkMove(const BaseTypes::Move& move) {
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
		std::string attackedSquares = python::extract<std::string>(validator.attr("getAttackedSquares")(attackerSquareIndex));
		int numberOfZeroesToAdd = 64 - attackedSquares.size();
		for (int i = 0; i < numberOfZeroesToAdd; i++) {
			attackedSquares += "0";
		}
        BaseTypes::Bitboard result(attackedSquares);
        return result;
    } catch (const python::error_already_set) {
        PyErr_Print();
        assert(false);
    }
}

BaseTypes::Bitboard PythonChessValidator::getBitboard() {
    try {
        std::string bitboard = python::extract<std::string>(validator.attr("getBitboard")());
        return BaseTypes::Bitboard(bitboard);
    } catch (const python::error_already_set) {
        PyErr_Print();
        assert(false);
    }
}
