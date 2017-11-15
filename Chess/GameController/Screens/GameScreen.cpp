//
//  GameScreen.cpp
//  Chess
//
//  Created by macbook on 11/2/17.
//  Copyright © 2017 Tran Hoang Duong. All rights reserved.
//

#include "GameScreen.h"
#include "OfflineGame.h"
#include <iostream>

GameScreen* GameScreen::create(OfflineGame* game, int width, int height) {
    if (!displayer) {
        displayer = DisplayerImplementation::getInstance();
    }
    
    GameScreen* screen = new GameScreen();
    screen->game = game;
    screen->game->setDelegate(screen);
    screen->width = width;
    screen->height = height;
    return screen;
}

void GameScreen::onEnter() {
    std::cout << "GameScreen - onEnter" << std::endl;
    game->start(BaseTypes::Side::WHITE, 1);
}

void GameScreen::onExit() {
    std::cout << "GameScreen - onExit" << std::endl;
    delete game;
    delete this;
}

void GameScreen::onBoardInitStateInvalid(const std::vector<BaseTypes::Position>& misplacedPositions) {
    print(1, "Pieces are misplaced: ");
    std::string positions = "";
    std::vector<BaseTypes::Position> misplacedPositions_ = misplacedPositions;
    for (int i = 0; i < misplacedPositions_.size(); i++) {
        positions += misplacedPositions_.at(i).toString();
        positions += " ";
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

void GameScreen::onWinGame(const WinGameData& data) {
    
}

void GameScreen::onLoseGame(const LoseGameData& data) {
    
}

void GameScreen::onDrawGame(const DrawGameData& data) {
    
}

void GameScreen::onInvalidAction(const InvalidActionData& data) {
    
}
