//
//  PythonChessValidator.cpp
//  Chess
//
//  Created by macbook on 10/10/17.
//  Copyright © 2017 Tran Hoang Duong. All rights reserved.
//

#include "PythonChessValidator.h"

const char* PYTHONPATH = ".";

PythonChessValidator* PythonChessValidator::instance = NULL;

PythonChessValidator* PythonChessValidator::getInstance() {
    if (!instance) {
        instance = new PythonChessValidator();
        instance->init();
    }
    return instance;
}

void PythonChessValidator::init() {
    setenv("PYTHONPATH", PYTHONPATH, 1);
    Py_Initialize();
    try {
        python::object my_python_class_module = python::import("ChessValidator");
        validator = my_python_class_module.attr("Validator")();
    } catch (const python::error_already_set) {
        PyErr_Print();
        assert(false);
    }
}

void PythonChessValidator::start() {
    validator.attr("init")();
}

bool PythonChessValidator::checkMove(const BaseTypes::Move& move) {
    bool result;
    try {
        result = python::extract<bool>(validator.attr("isMoveValid")(BaseTypes::Move(move).toString()));
    } catch (const python::error_already_set) {
        PyErr_Print();
        result = false;
    }
    return result;
}

bool PythonChessValidator::checkGameOver() {
    bool result;
    try {
        result = python::extract<bool>(validator.attr("isGameOver"));
    } catch (const python::error_already_set) {
        PyErr_Print();
        result = false;
    }
    return result;
}

bool PythonChessValidator::checkDraw() {
    bool result;
    try {
        result = python::extract<bool>(validator.attr("isDrawGame"));
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
        validator.attr("move")(BaseTypes::Move(move).toString());
    } catch (const python::error_already_set) {
        PyErr_Print();
        assert(false);
    }
}
