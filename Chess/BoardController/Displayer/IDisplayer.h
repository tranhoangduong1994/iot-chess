//
//  IDisplayer.h
//  Chess
//
//  Created by macbook on 11/5/17.
//  Copyright © 2017 Tran Hoang Duong. All rights reserved.
//

#ifndef IDisplayer_h
#define IDisplayer_h

#include <string>

class IDisplayer {
public:
    virtual void print(int line, std::string content) = 0;
    virtual void print(int line, int position, std::string content) = 0;
    virtual void clear() = 0;
};

#endif /* IDisplayer_h */
