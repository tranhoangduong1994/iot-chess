//
//  Displayer.cpp
//  Chess
//
//  Created by macbook on 11/5/17.
//  Copyright © 2017 Tran Hoang Duong. All rights reserved.
//

#include "Displayer.h"
#include "BoardServices.h"
#include "wiringPiI2C.h"
#include <thread>

const std::string CLEARING_STRING = "                    ";

#define I2C_ADDR 0x3f

#define ENABLE  0b00000100 // Enable bit

#define LCD_CHR 1
#define LCD_CMD 0

#define LCD_CLEARDISPLAY 0x01
#define LCD_RETURNHOME 0x02
#define LCD_ENTRYMODESET 0x04
#define LCD_DISPLAYCONTROL 0x08
#define LCD_CURSORSHIFT 0x10
#define LCD_FUNCTIONSET 0x20
#define LCD_SETCGRAMADDR 0x40
#define LCD_SETDDRAMADDR 0x80

#define LCD_ENTRYRIGHT 0x00
#define LCD_ENTRYLEFT 0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00

#define LCD_DISPLAYON 0x04
#define LCD_DISPLAYOFF 0x00
#define LCD_CURSORON 0x02
#define LCD_CURSOROFF 0x00
#define LCD_BLINKON 0x01
#define LCD_BLINKOFF 0x00

#define LCD_DISPLAYMOVE 0x08
#define LCD_CURSORMOVE 0x00
#define LCD_MOVERIGHT 0x04
#define LCD_MOVELEFT 0x00

#define LCD_8BITMODE 0x10
#define LCD_4BITMODE 0x00
#define LCD_2LINE 0x08
#define LCD_1LINE 0x00
#define LCD_5x10DOTS 0x04
#define LCD_5x8DOTS 0x00

#define LCD_BACKLIGHT 0x08
#define LCD_NOBACKLIGHT 0x00

void Displayer::init() {
    //    lcd = PythonHelper::getInstance()->createObject("I2C_LCD_driver", "lcd");
    
    wiringPiSetup();
    fd = wiringPiI2CSetup(I2C_ADDR);
    
    lcdWrite(0x03, LCD_CMD);
    lcdWrite(0x03, LCD_CMD);
    lcdWrite(0x03, LCD_CMD);
    lcdWrite(0x02, LCD_CMD);
    
    lcdWrite(LCD_FUNCTIONSET | LCD_2LINE | LCD_5x8DOTS | LCD_4BITMODE, LCD_CMD);
    lcdWrite(LCD_DISPLAYCONTROL | LCD_DISPLAYON, LCD_CMD);
    lcdWrite(LCD_CLEARDISPLAY, LCD_CMD);
    lcdWrite(LCD_ENTRYMODESET, LCD_ENTRYLEFT);
    
    std::this_thread::sleep_for(std::chrono::microseconds(200));
}

void Displayer::lcdStrobe(int data) {
    wiringPiI2CReadReg8(fd, (data | ENABLE | LCD_BACKLIGHT));
    std::this_thread::sleep_for(std::chrono::microseconds(500));
    wiringPiI2CReadReg8(fd, (data & ~ENABLE) | LCD_BACKLIGHT);
    std::this_thread::sleep_for(std::chrono::microseconds(100));
}

void Displayer::lcdWriteFourBits(int data) {
    wiringPiI2CReadReg8(fd, data | LCD_BACKLIGHT);
    lcdStrobe(data);
}

void Displayer::lcdWrite(int data, int mode) {
    lcdWriteFourBits(mode | (data & 0xF0));
    lcdWriteFourBits(mode | ((data << 4) & 0xF0));
}


Displayer* Displayer::instance = NULL;

Displayer* Displayer::getInstance() {
    if (!instance) {
        instance = new Displayer();
        instance->init();
    }
    return instance;
}

void Displayer::print(int lineNumber, std::string content) {
    print(int lineNumber, 0, content);
}

void Displayer::print(int lineNumber, int position, std::string content) {
    int pos;
    if (lineNumber == 1) {
        pos = position;
    } else if (lineNumber == 2) {
        pos = 0x40 + position;
    } else if (lineNumber == 3) {
        pos = 0x14 + position;
    } else if (lineNumber == 4) {
        pos = 0x54 + position;
    }
    
    lcdWrite(pos, LCD_CMD);
    const char* s = content.c_str();
    while (*s) {
        lcdWrite(*(s++), LCD_CHR);
    }
}

void Displayer::clear() {
//    lcd.attr("lcd_clear")();
    lcdWrite(LCD_CLEARDISPLAY, LCD_CMD);
    lcdWrite(LCD_RETURNHOME, LCD_CMD);
}
