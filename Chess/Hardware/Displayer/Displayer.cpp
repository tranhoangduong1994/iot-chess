//
//  Displayer.cpp
//  Chess
//
//  Created by macbook on 11/5/17.
//  Copyright © 2017 Tran Hoang Duong. All rights reserved.
//

#include "Displayer.h"
#include "BoardServices.h"

const std::string CLEARING_STRING = "                    ";

Displayer* Displayer::instance = NULL;

Displayer* Displayer::getInstance() {
    if (!instance) {
        instance = new Displayer();
        instance->init();
    }
    return instance;
}

void Displayer::print(int lineNumber, std::string content) {
    std::cout << "[Displayer] print(" << lineNumber << ", " << content << ")" << std::endl;
//    lcd.attr("lcd_display_string")(CLEARING_STRING, lineNumber);
//    std::cout << "[Displayer] print - 1";
    lcd.attr("lcd_display_string")(content, lineNumber);
    std::cout << "[Displayer] print - 2";
}

void Displayer::print(int lineNumber, int position, std::string content) {
    lcd.attr("lcd_display_string")(lineNumber, position, content);
}

void Displayer::clear() {
    lcd.attr("lcd_clear")();
}

void Displayer::init() {
    lcd = PythonHelper::getInstance()->createObject("I2C_LCD_driver", "lcd");
}
