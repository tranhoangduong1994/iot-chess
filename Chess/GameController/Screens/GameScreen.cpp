//
//  GameScreen.cpp
//  Chess
//
//  Created by macbook on 11/2/17.
//  Copyright © 2017 Tran Hoang Duong. All rights reserved.
//

#include "GameScreen.h"

#include <iostream>

#include "OfflineGame.h"
#include "OptionScreen.h"

GameScreen* GameScreen::create(OfflineGame* game) {
    if (!displayer) {
        displayer = DisplayerImplementation::getInstance();
    }
    
    GameScreen* screen = new GameScreen();
    screen->game = game;
    screen->init();
    
    return screen;
}

void GameScreen::onEnter() {
    std::cout << "GameScreen - onEnter" << std::endl;
    if (!entered) {
        entered = true;
        game->start(BaseTypes::Side::WHITE, 1);
    }
    game->setDelegate(this);
}

void GameScreen::onExit() {
    std::cout << "GameScreen - onExit" << std::endl;
    delete game;
    delete this;
}

void GameScreen::init() {
    entered = false;
}

void GameScreen::onBoardInitStateInvalid(const BaseTypes::Bitboard& misplacedPositions) {
    print(1, "Pieces are misplaced: ");
    std::string positions = "";
    for (int i = 0; i < 64; i++) {
        if (misplacedPositions.get(i)) {
            positions += (char)((i % 8) + 97);
            positions += ((i / 8) + 1);
            positions += " ";
        }
    }
    print(2, positions);
}

void GameScreen::onGameStarted(const GameStartedData& data) {
    clearScreen();
    print(1, "Game started");
}

void GameScreen::onTurnBegan(const TurnBeganData& data) {
    if (data.opponent_move != "") {
        print(1, "Opponent move: " + data.opponent_move);
    }
    
    print(2, "It's your turn.");
    print(3, "Please make your move...");
    print(4, "");
}

void GameScreen::onTurnEnded(const TurnEndedData& data) {
    print(3, "You played: " + data.player_move);
    print(4, "Please wait for opponent move...");
}

void GameScreen::onInvalidMove(const InvalidMoveData& data) {
    print(3, "You play: " + data.player_move);
    print(4, "Invalid move, please try again: ");
}

void GameScreen::onMultipleMovesAvailable(const std::vector<BaseTypes::Move>& moves, std::function<void(bool, BaseTypes::Move)> onSelected) {
    std::vector<Entry> entries;
    for (int i = 0; i < moves.size(); i++) {
        Entry entry;
        entry.name = moves.at(i).toString();
        entry.onSelected = [=](std::string content) {
            onSelected(true, BaseTypes::Move(content));
        };
        entries.push_back(entry);
    }
    Entry cancelEntry;
    cancelEntry.name = "CANCEL";
    cancelEntry.onSelected = [=](BaseTypes::Move move) {
        onSelected(false, BaseTypes::Move());
    };
    entries.push_back(cancelEntry);
    
    OptionScreen* screen = OptionScreen::create("Which is your move?", entries);
    Screen::pushScreen(screen);
}


void GameScreen::onWinGame(const WinGameData& data) {
    
}

void GameScreen::onLoseGame(const LoseGameData& data) {
    
}

void GameScreen::onDrawGame(const DrawGameData& data) {
    
}

void GameScreen::onInvalidAction(const InvalidActionData& data) {
    
}
