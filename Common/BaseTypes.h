//
//  BaseTypes.h
//  Chess
//
//  Created by Tran Hoang Duong on 9/17/17.
//  Copyright © 2017 Tran Hoang Duong. All rights reserved.
//

#ifndef BaseTypes_h
#define BaseTypes_h

#include <string>
#include <vector>
#include <map>
#include <iostream>

typedef std::map<std::string, std::string> EventData;

namespace BaseTypes {
    enum Side {
        BLACK,
        WHITE
    };
    
    struct Position {
        char file;
        int rank;
        Position() {};
        Position(char file, int rank) {
            this->file = file;
            this->rank = rank;
        }
        Position(std::string fileRank) {
            char file = fileRank[0];
            int rank = atoi(&fileRank[1]);
            this->file = file;
            this->rank = rank;
        }
        const std::string toString() {
            return std::string(this->file + std::to_string(this->rank));
        }
    };
    
    struct Move {
        Position fromPos;
        Position toPos;
        char promotionType;
        Move() {};
        Move(Position fromPos, Position toPos) {
            this->fromPos = fromPos;
            this->toPos = toPos;
            this->promotionType = 0;
        }
        Move(Position fromPos, Position toPos, char pt) {
            this->fromPos = fromPos;
            this->toPos = toPos;
            this->promotionType = pt;
        }
        Move(std::string fromPosToPos) {
            char fromPosFile = fromPosToPos[0];
            int fromPosRank = atoi(&fromPosToPos[1]);
            char toPosFile = fromPosToPos[2];
            int toPosRank = atoi(&fromPosToPos[3]);
            char promotionType = 0;
            if (fromPosToPos.size() == 5) {
                promotionType = fromPosToPos[4];
            }
            Position fromPos = Position(fromPosFile, fromPosRank);
            Position toPos = Position(toPosFile, toPosRank);
            this->fromPos = fromPos;
            this->toPos = toPos;
            this->promotionType = promotionType;
        }
        const std::string toString() {
            return fromPos.toString() + toPos.toString() + promotionType;
        }
    };
}

#endif /* BaseTypes_h */
