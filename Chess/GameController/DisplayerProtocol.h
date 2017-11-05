//
//  DisplayerProtocol.h
//  Chess
//
//  Created by macbook on 11/5/17.
//  Copyright © 2017 Tran Hoang Duong. All rights reserved.
//

#ifndef DisplayerProtocol_h
#define DisplayerProtocol_h

#include <string>

class DisplayerProtocol {
public:
    virtual void print(int line, std::string content) = 0;
    virtual void clear() = 0;
};

#endif /* DisplayerProtocol_h */
