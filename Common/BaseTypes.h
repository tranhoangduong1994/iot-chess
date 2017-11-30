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
        const std::string toString() const {
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
        const std::string toString() const {
            return fromPos.toString() + toPos.toString() + promotionType;
        }
    };
    
    struct Bitboard {
        unsigned long first32;
        unsigned long last32;
        
        Bitboard() {
            first32 = 0;
            last32 = 0;
        }
        
        Bitboard(std::string string) {
            first32 = 0;
            last32 = 0;
            
            if (string.size() != 64) {
                return;
            }
            
            int iMax = (64 < string.size() ? 64 : string.size());
            
            for (int i = 0; i < iMax; i++) {
                if (string[i] == '0') {
                    set(i, 0);
                    continue;
                }
                
                if (string[i] == '1') {
                    set(i, 1);
                    continue;
                }
                
                first32 = 0;
                last32 = 0;
                return;
            }
        }
        
        bool get(int index) const {
            if (index > 31) {
                return last32 & (1 << (index - 1));
            }
            
            return first32 & (1 << (index - 1));
        }
        
        void set(int index, unsigned long value) {
            if (index > 63 || index < 0) {
                return;
            }
            
            if (index > 31) {
                last32 &= ~(value << (index - 32));
                last32 |= (value << (index - 32));
                return;
            }
            
            first32 &= ~(value << index);
            first32 |= (value << index);
        }
        
        int popCount() {
            int first32Count = 0, last32Count = 0;
            unsigned long f32 = first32, l32 = last32;
            while(f32 > 0 && l32 > 0) {
                if ((f32 & 1) == 1) {
                    first32Count++;
                }
                f32 >>= 1;
                
                if ((l32 & 1) == 1) {
                    last32Count++;
                }
                l32 >>= 1;
            }
            return first32Count + last32Count;
        }
        
        const char* toString() {
            static char string[65] = {0};
            unsigned long f32 = first32, l32 = last32;
            
            for (int i = 0; i < 32; i++) {
                string[i] = (f32 & 1 ? '1' : '0');
                f32 = f32 >> 1;
                
                string[i + 32] = (l32 & 1 ? '1' : '0');
                l32 = l32 >> 1;
            }
            
            return string;
        }
        
        bool operator== (const Bitboard& other) {
            return this->first32 == other.first32 && this->last32 == other.last32;
        }
        
        bool operator!= (const Bitboard& other) {
            return this->first32 != other.first32 || this->last32 != other.last32;
        }

        friend BaseTypes::Bitboard operator^(const Bitboard& l, const Bitboard& r) {
            Bitboard result;
            result.first32 = l.first32 ^ r.first32;
            result.last32 = l.last32 ^ r.last32;
            return result;
        }
        
        friend BaseTypes::Bitboard operator&(const Bitboard& l, const Bitboard& r) {
            Bitboard result;
            result.first32 = l.first32 & r.first32;
            result.last32 = l.last32 & r.last32;
            return result;
        }
        
        friend BaseTypes::Bitboard operator<< (const Bitboard& bitboard, int bit) {
            Bitboard result;
            result.first32 = bitboard.first32 << bit | bitboard.last32 >> (32 - bit);
            result.last32 = bitboard.last32 << bit;
            return result;
        }
        
        friend BaseTypes::Bitboard operator>> (const Bitboard& bitboard, int bit) {
            Bitboard result;
            result.first32 = bitboard.first32 >> bit;
            result.last32 = bitboard.last32 >> bit | bitboard.first32 << (32 - bit);
            return result;
        }
        
        Bitboard getMisplacedPositions(const Bitboard& other) const {
            return *this & (*this ^ other);
        }
        
        int getIndexOfSetBit(int nth) {
            int occurrenceTime = 0;
            int index = -1;
            unsigned long f32 = first32, l32 = last32;
            while(f32) {
                index++;
                if (f32 & 1) {
                    occurrenceTime++;
                }
                
                if (occurrenceTime == nth) {
                    return 63 - index;
                }
                
                f32 >>= 1;
            }
            
            while(l32) {
                index++;
                if (l32 & 1) {
                    occurrenceTime++;
                }
                
                if (occurrenceTime == nth) {
                    return 63 - index;
                }
                
                l32 >>= 1;
            }
            
            return -1;
        }
    };
}

#endif /* BaseTypes_h */
