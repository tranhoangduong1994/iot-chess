//
//  Displayer.h
//  Chess
//
//  Created by macbook on 11/5/17.
//  Copyright © 2017 Tran Hoang Duong. All rights reserved.
//

#ifndef Displayer_h
#define Displayer_h

#include <string>

class Displayer {
public:
    static Displayer* getInstance();
    
    void print(int lineNumber, std::string content);
    void print(int lineNumber, int position, std::string content);
    void clear();
    
private:
    static Displayer* instance;
    
    int fd;
    
    void init();
    
    void lcdStrobe(int data);
    void lcdWriteFourBits(int data);
    void lcdWrite(int data, int mode);
};

#endif /* Displayer_h */
