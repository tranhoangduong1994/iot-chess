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
    validator = PythonHelper::getInstance()->createObject("ChessValidator", "Validator");
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

BaseTypes::PieceType PythonChessValidator::getPieceType(int squareIndex) {
    try {
        std::string typeString = python::extract<std::string>(validator.attr("getPieceType")(squareIndex));
        if (typeString == "K") {
            return BaseTypes::PieceType::WHITE_KING;
        }
        if (typeString == "Q") {
            return BaseTypes::PieceType::WHITE_QUEEN;
        }
        if (typeString == "B") {
            return BaseTypes::PieceType::WHITE_BISHOP;
        }
        if (typeString == "N") {
            return BaseTypes::PieceType::WHITE_KNIGHT;
        }
        if (typeString == "R") {
            return BaseTypes::PieceType::WHITE_ROOK;
        }
        if (typeString == "P") {
            return BaseTypes::PieceType::WHITE_PAWN;
        }
        if (typeString == "k") {
            return BaseTypes::PieceType::BLACK_KING;
        }
        if (typeString == "q") {
            return BaseTypes::PieceType::BLACK_QUEEN;
        }
        if (typeString == "b") {
            return BaseTypes::PieceType::BLACK_BISHOP;
        }
        if (typeString == "n") {
            return BaseTypes::PieceType::BLACK_KNIGHT;
        }
        if (typeString == "p") {
            return BaseTypes::PieceType::BLACK_PAWN;
        }
        return BaseTypes::NONE;
    } catch (const python::error_already_set) {
        PyErr_Print();
        assert(false);
    }
}
