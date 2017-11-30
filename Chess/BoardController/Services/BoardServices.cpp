//
//  BoardServicesImplementation.cpp
//  Chess
//
//  Created by macbook on 11/4/17.
//  Copyright © 2017 Tran Hoang Duong. All rights reserved.
//

#include "BoardServices.h"

#include "MessageController.h"

BoardServices* BoardServices::instance = NULL;

BoardServices* BoardServices::getInstance() {
    if (!instance) {
        instance = new BoardServices();
        instance->init();
    }
    return instance;
}

void BoardServices::clearScreen() {
    MessageController::getInstance()->send(ServiceRequestType::CLEAR_SCREEN);
}
                
void BoardServices::display(int line, std::string string) {
    MessageController::getInstance()->send(ServiceRequestType::PRINT, std::to_string(line) + string);
}

void BoardServices::move(BaseTypes::Move move) {
    MessageController::getInstance()->send(ServiceRequestType::MOVE, move.toString());
}

void BoardServices::scan() {
    MessageController::getInstance()->send(ServiceRequestType::SCAN_BOARD);
}

void BoardServices::resetBoard() {
    MessageController::getInstance()->send(ServiceRequestType::RESET_BOARD);
}

void BoardServices::setBoardSystemEventsDelegate(BoardSystemEventsProtocol* s_delegate) {
    MessageController::getInstance()->setBoardSystemEventsDelegate(s_delegate);
}

void BoardServices::setBoardIngameEventsDelegate(BoardIngameEventsProtocol* g_delegate) {
    MessageController::getInstance()->setBoardIngameEventsDelegate(g_delegate);
}

void BoardServices::setBoardKeyEventsDelegate(BoardKeyEventsProtocol* k_delegate) {
    MessageController::getInstance()->setBoardKeyEventsDelegate(k_delegate);
}

void BoardServices::init() {
    
}
