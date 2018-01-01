//
//  StockfishWrapper.cpp
//  Chess
//
//  Created by macbook on 10/30/17.
//  Copyright © 2017 Tran Hoang Duong. All rights reserved.
//

#include "StockfishWrapper.h"

#include "bitboard.h"
#include "position.h"
#include "search.h"
#include "thread.h"
#include "tt.h"
#include "timeman.h"
#include "uci.h"
#include "syzygy/tbprobe.h"

#include <sstream>
#include <iostream>

namespace PSQT {
    void init();
}

namespace UCI {
    void init(OptionsMap&);
}

namespace {
    
    // FEN string of the initial position, normal chess
    const char* StartFEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
    
    Position pos;
    
    // A list to keep track of the position states along the setup moves (from the
    // start position to the position just before the search starts). Needed by
    // 'draw by repetition' detection.
    StateListPtr States(new std::deque<StateInfo>(1));
    
    
    // position() is called when engine receives the "position" UCI command.
    // The function sets up the position described in the given FEN string ("fen")
    // or the starting position ("startpos") and then makes the moves given in the
    // following move list ("moves").
    
    void position(Position& pos, std::istringstream& is) {
        
        Move m;
        std::string token, fen;
        
        is >> token;
        
        if (token == "startpos")
        {
            fen = StartFEN;
            is >> token; // Consume "moves" token if any
        }
        else if (token == "fen")
            while (is >> token && token != "moves")
                fen += token + " ";
        else
            return;
        
        States = StateListPtr(new std::deque<StateInfo>(1));
        pos.set(fen, Options["UCI_Chess960"], &States->back(), Threads.main());
        
        // Parse move list (if any)
        while (is >> token && (m = StockfishWrapper::to_move(pos, token)) != MOVE_NONE)
        {
            States->push_back(StateInfo());
            pos.do_move(m, States->back());
        }
    }
    
    
    // setoption() is called when engine receives the "setoption" UCI command. The
    // function updates the UCI option ("name") to the given value ("value").
    
    void setoption(std::istringstream& is) {
        
        std::string token, name, value;
        
        is >> token; // Consume "name" token
        
        // Read option name (can contain spaces)
        while (is >> token && token != "value")
            name += std::string(" ", name.empty() ? 0 : 1) + token;
        
        // Read option value (can contain spaces)
        while (is >> token)
            value += std::string(" ", value.empty() ? 0 : 1) + token;
        
        if (Options.count(name))
            Options[name] = value;
        else
            sync_cout << "No such option: " << name << sync_endl;
    }
    
    
    // go() is called when engine receives the "go" UCI command. The function sets
    // the thinking time and other parameters from the input string, then starts
    // the search.
    
    void go(Position& pos, std::istringstream& is) {
        
        Search::LimitsType limits;
        std::string token;
        
        limits.startTime = now(); // As early as possible!
        
        while (is >> token)
            if (token == "searchmoves")
                while (is >> token)
                    limits.searchmoves.push_back(StockfishWrapper::to_move(pos, token));
        
            else if (token == "wtime")     is >> limits.time[WHITE];
            else if (token == "btime")     is >> limits.time[BLACK];
            else if (token == "winc")      is >> limits.inc[WHITE];
            else if (token == "binc")      is >> limits.inc[BLACK];
            else if (token == "movestogo") is >> limits.movestogo;
            else if (token == "depth")     is >> limits.depth;
            else if (token == "nodes")     is >> limits.nodes;
            else if (token == "movetime")  is >> limits.movetime;
            else if (token == "mate")      is >> limits.mate;
            else if (token == "infinite")  limits.infinite = 1;
            else if (token == "ponder")    limits.ponder = 1;
        
        Threads.start_thinking(pos, States, limits);
    }
    
    // On ucinewgame following steps are needed to reset the state
    void newgame() {
        
        TT.resize(Options["Hash"]);
        Search::clear();
        Tablebases::init(Options["SyzygyPath"]);
        Time.availableNodes = 0;
    }
    
} // namespace

/// UCI::value() converts a Value to a string suitable for use with the UCI
/// protocol specification:
///
/// cp <x>    The score from the engine's point of view in centipawns.
/// mate <y>  Mate in y moves, not plies. If the engine is getting mated
///           use negative values for y.

std::string StockfishWrapper::value(Value v) {
    
    assert(-VALUE_INFINITE < v && v < VALUE_INFINITE);
    
    std::stringstream ss;
    
    if (abs(v) < VALUE_MATE - MAX_PLY)
        ss << "cp " << v * 100 / PawnValueEg;
    else
        ss << "mate " << (v > 0 ? VALUE_MATE - v + 1 : -VALUE_MATE - v) / 2;
    
    return ss.str();
}


/// UCI::square() converts a Square to a string in algebraic notation (g1, a7, etc.)

std::string StockfishWrapper::square(Square s) {
    return std::string{ char('a' + file_of(s)), char('1' + rank_of(s)) };
}


/// UCI::move() converts a Move to a string in coordinate notation (g1f3, a7a8q).
/// The only special case is castling, where we print in the e1g1 notation in
/// normal chess mode, and in e1h1 notation in chess960 mode. Internally all
/// castling moves are always encoded as 'king captures rook'.

std::string StockfishWrapper::move(Move m, bool chess960) {
    
    Square from = from_sq(m);
    Square to = to_sq(m);
    
    if (m == MOVE_NONE)
        return "(none)";
    
    if (m == MOVE_NULL)
        return "0000";
    
    if (type_of(m) == CASTLING && !chess960)
        to = make_square(to > from ? FILE_G : FILE_C, rank_of(from));
    
    std::string move = StockfishWrapper::square(from) + StockfishWrapper::square(to);
    
    if (type_of(m) == PROMOTION)
        move += " pnbrqk"[promotion_type(m)];
    
    return move;
}


/// UCI::to_move() converts a string representing a move in coordinate notation
/// (g1f3, a7a8q) to the corresponding legal Move, if any.

Move StockfishWrapper::to_move(const Position& pos, std::string& str) {
    
    if (str.length() == 5) // Junior could send promotion piece in uppercase
        str[4] = char(tolower(str[4]));
    
    for (const auto& m : MoveList<LEGAL>(pos))
        if (str == StockfishWrapper::move(m, pos.is_chess960()))
            return m;
    
    return MOVE_NONE;
}

StockfishWrapper* StockfishWrapper::instance = NULL;

StockfishWrapper* StockfishWrapper::getInstance() {
    if (!instance) {
        instance = new StockfishWrapper();
        instance->init();
    }
    
    return instance;
}

std::function<void(BaseTypes::Move bestMove)>& StockfishWrapper::getCaculatingCallback() {
    return this->onCalculated;
}

void StockfishWrapper::init() {
    UCI::init(Options);
    PSQT::init();
    Bitboards::init();
    Position::init();
    Bitbases::init();
    Search::init();
    Pawns::init();
    Threads.init();
}

void StockfishWrapper::start(int difficulty) {
    gameMoves = "startpos moves ";
    
    //ucinewgame
    std::string token, cmd;
    newgame();
    pos.set(StartFEN, false, &States->back(), Threads.main());
    
    //set difficulty;
    std::string difficultyString = "name Skill Level value " + std::to_string(difficulty);
    std::istringstream is(difficultyString);
    setoption(is);
}

void StockfishWrapper::move(BaseTypes::Move move) {
    //add new move to gameMoves
    std::string fromPosRank = std::to_string(move.fromPos.rank);
    std::string toPosRank = std::to_string(move.toPos.rank);
    gameMoves += (move.fromPos.file + fromPosRank + move.toPos.file + toPosRank + " ");
    
    std::istringstream is(gameMoves);
    position(pos, is);
}

void StockfishWrapper::calculate(std::function<void(BaseTypes::Move bestMove)> onCalculated) {
    std::istringstream is(gameMoves + " depth 3");
    go(pos, is);
    this->onCalculated = onCalculated;
}

void StockfishWrapper::log() {
    std::cout << pos << std::endl;
}
