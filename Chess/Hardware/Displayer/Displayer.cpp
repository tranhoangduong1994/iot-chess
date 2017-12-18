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
    }
    return instance;
}

void Displayer::print(int lineNumber, std::string content) {
    lcd.attr("lcd_display_string")(CLEARING_STRING, line);
    lcd.attr("lcd_display_string")(content, line);
}

void Displayer::print(int lineNumber, int position, std::string content) {
    lcd.attr("lcd_display_string")(line, position, content);
}

void Displayer::clear() {
    lcd.attr("lcd_clear")();
}