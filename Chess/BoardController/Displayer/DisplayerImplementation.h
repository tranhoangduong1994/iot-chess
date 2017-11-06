//
//  DisplayerImplementation.h
//  Chess
//
//  Created by macbook on 11/5/17.
//  Copyright © 2017 Tran Hoang Duong. All rights reserved.
//

#ifndef DisplayerImplementation_h
#define DisplayerImplementation_h

#include "DisplayerProtocol.h"

class DisplayerImplementation : public DisplayerProtocol {
public:
    static DisplayerImplementation* getInstance();
    
    void print(int lineNumber, std::string content) override;
    void clear() override;
    
private:
    static DisplayerImplementation* instance;
};

#endif /* DisplayerImplementation_h */
