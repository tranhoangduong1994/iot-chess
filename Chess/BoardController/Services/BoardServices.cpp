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

void BoardServices::init() {
	std::cout << "[BoardServices] init" << std::endl;
    lcd = PythonWrapper::getInstance()->createObject("I2C_LCD_driver", "lcd");
}

void BoardServices::clearScreen() {
	lcd.attr("lcd_clear")();
}
                
void BoardServices::display(int line, std::string string) {
	lcd.attr("lcd_display_string")(string, line);
}

void BoardServices::display(int line, int position, std::string string) {
	lcd.attr("lcd_display_string")(string, line, position);
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

void BoardServices::setBoardSystemEventsDelegate(BoardSystemEventsProtocol* s_delegate) {
    MessageController::getInstance()->setBoardSystemEventsDelegate(s_delegate);
}

void BoardServices::setBoardServicesEventsDelegate(BoardServicesEventsProtocol* g_delegate) {
    MessageController::getInstance()->setBoardServicesEventsDelegate(g_delegate);
}

void BoardServices::setBoardKeyEventsDelegate(BoardKeyEventsProtocol* k_delegate) {
    MessageController::getInstance()->setBoardKeyEventsDelegate(k_delegate);
}
