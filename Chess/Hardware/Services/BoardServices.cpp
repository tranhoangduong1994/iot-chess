//
//  BoardServicesImplementation.cpp
//  Chess
//
//  Created by macbook on 11/4/17.
//  Copyright © 2017 Tran Hoang Duong. All rights reserved.
//

#include "BoardServices.h"

#include "MessageController.h"
#include "PythonHelper.h"

BoardServices* BoardServices::instance = NULL;

BoardServices* BoardServices::getInstance() {
    if (!instance) {
        instance = new BoardServices();
        instance->init();
    }
    return instance;
}

void BoardServices::init() {
	std::cout << "[BoardServices] init" << std::endl;
}

void BoardServices::move(BaseTypes::Move move) {
    MessageController::getInstance()->send(ServiceRequestType::MOVE, move.toString());
}

void BoardServices::castling(CastlingType type) {
	MessageController::getInstance()->send(ServiceRequestType::CASTLING, std::to_string(type));
}

void BoardServices::scan() {
    MessageController::getInstance()->send(ServiceRequestType::SCAN_BOARD);
}

void BoardServices::resetBoard() {
    MessageController::getInstance()->send(ServiceRequestType::RESET_BOARD);
}

void BoardServices::playSound(SoundType type) {
    MessageController::getInstance()->send(ServiceRequestType::PLAY_SOUND, std::to_string(type));
}

void BoardServices::setBoardSystemEventsDelegator(BoardSystemEventsProtocol* delegator) {
    MessageController::getInstance()->setBoardSystemEventsDelegator(delegator);
}

void BoardServices::setBoardServicesEventsDelegator(BoardServicesEventsProtocol* delegator) {
    MessageController::getInstance()->setBoardServicesEventsDelegator(delegator);
}
