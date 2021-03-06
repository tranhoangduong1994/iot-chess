//
//  StockfishEngine.cpp
//  Chess
//
//  Created by Tran Hoang Duong on 9/17/17.
//  Copyright © 2017 Tran Hoang Duong. All rights reserved.
//

#include "StockfishEngine.h"
#include "StockfishWrapper.h"
#include "PythonChessValidator.h"

#include <iostream>
#include <string>

StockfishEngine* StockfishEngine::instance = NULL;

StockfishEngine* StockfishEngine::getInstance() {
    if (!instance) {
        instance = new StockfishEngine();
        instance->init();
    }
    
    return instance;
}

void StockfishEngine::start(int difficulty) {
    wrapper = StockfishWrapper::getInstance();
    wrapper->start(difficulty);
}

bool StockfishEngine::move(const BaseTypes::Move& move) {
    wrapper->move(move);
    
    return true;
}

void StockfishEngine::calculate(std::function<void(BaseTypes::Move bestMove)> onCalculated) {
    wrapper->calculate(onCalculated);
}

void StockfishEngine::log() {
    wrapper->log();
}

void StockfishEngine::init() {

}
